#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/system_description.h"

namespace ecs
{
  template<typename E>
  struct EventDescription;
  template<typename E>
  struct SingleEventDescription;

  struct Core
  {
    std::unordered_map<uint, FullTypeDescription> types;
    std::vector<Archetype*> archetypes;
    std::vector<SystemDescription*> systems;
    std::vector<QueryDescription*> queries;
    std::vector<QueryDescription*> event_queries;
    EntityContainer entityContainer;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    Core();
    ~Core();
    template<typename E>
    std::vector<EventDescription<E>*> &events_handler()
    {
      static std::vector<EventDescription<E>*> handlers;
      return handlers;
    }
    template<typename E>
    std::vector<SingleEventDescription<E>*> &single_events_handler()
    {
      static std::vector<SingleEventDescription<E>*> handlers;
      return handlers;
    }
    void destroy_entities();
  };
  Core &core();

  template<typename E>
  struct EventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&);
    EventHandler eventHandler;
    EventDescription(const char *name, const std::vector<FunctionArgument> &args, EventHandler eventHandler):
      QueryDescription(name, args, false), eventHandler(eventHandler)
    {
      core().events_handler<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
      printf("EventDescription %s\n", name);
    }
  };
  template<typename E>
  struct SingleEventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&, ecs::QueryIterator&);
    EventHandler eventHandler;
    SingleEventDescription(const char *name, const std::vector<FunctionArgument> &args, EventHandler eventHandler):
      QueryDescription(name, args, false), eventHandler(eventHandler)
    {
      core().single_events_handler<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
    }
  };


  template<typename T>
  T* get_component(const EntityId &entity, const char *name);

  Archetype *add_archetype(const ComponentTypes &types, int capacity);
  EntityId create_entity(ComponentInitializerList &list);
  void destroy_entity(const EntityId &eid);



  template<typename E>
  void send_event(const E &event)
  {
    core().events.push([event](){
    for (EventDescription<E> *descr : core().events_handler<E>())
      descr->eventHandler(event);
    });
  }


  bool get_iterator(const QueryDescription &descr, const EntityId &eid, QueryIterator &iterator);
  bool get_iterator(const SingleQueryDescription &descr, const EntityId &eid, QueryIterator &iterator);


  template<typename E>
  void send_event(const EntityId &eid, const E &event)
  {
    if (eid)
    {
      core().events.push([eid, event](){
        for (SingleEventDescription<E> *descr : core().single_events_handler<E>())
        {
          QueryIterator iterator;
          if (get_iterator(*((QueryDescription*)descr), eid, iterator))
          {
            descr->eventHandler(event, iterator);
          }
        } 
      });
    }
  }
  template<typename E>
  void send_event_immediate(const EntityId &eid, const E &event)
  {
    if (eid)
    {
      for (SingleEventDescription<E> *descr : core().single_events_handler<E>())
      {
        QueryIterator iterator;
        if (get_iterator(*((QueryDescription*)descr), eid, iterator))
        {
          descr->eventHandler(event, iterator);
        }
      } 
    }
  }
  template<typename E>
  void send_event_immediate(const E &event)
  {
    for (EventDescription<E> *descr : core().events_handler<E>())
      descr->eventHandler(event);
  }

  void system_statistic();

}
