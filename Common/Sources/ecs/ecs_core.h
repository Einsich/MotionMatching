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


}
