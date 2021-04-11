#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/string_hash.h"
#include "manager/system_description.h"
#include "manager/entity_id.h"
namespace ecs
{
  template<typename E>
  struct EventDescription;
  template<typename E>
  struct SingleEventDescription;

  struct Core
  {
    uint globalTypeIndex = 0;
    std::vector<FullTypeDescription> types;
    std::vector<Archetype*> archetypes;
    std::vector<SystemDescription*> systems;
    std::vector<QueryDescription*> queries;
    std::vector<QueryDescription*> event_queries;
    EntityContainer entityContainer;
    std::queue<std::function<void()>> events;
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
  };
  Core &core();

  template<typename E>
  struct EventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&);
    EventHandler eventHandler;
    EventDescription(const std::vector<FunctionArgument> &args, EventHandler eventHandler):
      QueryDescription(args, false), eventHandler(eventHandler)
    {
      core().events_handler<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
    }
  };
  template<typename E>
  struct SingleEventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&, ecs::QueryIterator&);
    EventHandler eventHandler;
    SingleEventDescription(const std::vector<FunctionArgument> &args, EventHandler eventHandler):
      QueryDescription(args, false), eventHandler(eventHandler)
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
    core().events.emplace([event](){
    for (EventDescription<E> *descr : core().events_handler<E>())
      descr->eventHandler(event);
    });
  }


  template<typename E>
  void send_event(const EntityId &id, const E &event)
  {
    if (id)
    {
      core().events.emplace([id, event](){
        int archetypeInd = id.archetype_index();
        int index = id.array_index();
        for (SingleEventDescription<E> *descr : core().single_events_handler<E>())
        {
          Archetype *archetype = core().archetypes[archetypeInd];

          auto it = std::find_if(descr->archetypes.begin(), descr->archetypes.end(),
            [archetype](const SystemCashedArchetype &cashed_archetype){return cashed_archetype.archetype == archetype;});
          if (it != descr->archetypes.end())
          {
            QueryIterator iterator(*((QueryDescription*)descr), it - descr->archetypes.begin(), index);
            descr->eventHandler(event, iterator);
          }
        } 
      });
    }
  }


}
