#pragma once
#include <vector>
#include "manager/string_hash.h"
#include "manager/system_description.h"
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
    Core() = default;
    ~Core();
    template<typename E>
    std::vector<EventDescription<E>*> &events()
    {
      static std::vector<EventDescription<E>*> eventPull;
      return eventPull;
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
      core().events<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
    }
  };

  class Entity
  {
  private:
    const static unsigned int index_mask = 0x0000ffff;
    unsigned int index;//archetype index + index in archetype
  public:
    unsigned int archetype_index() const;
    unsigned int array_index() const;

  };

  template<typename T>
  T* get_component(Entity entity, const char *name);


  void initialize_ecs();

  Archetype *add_archetype(const ComponentTypes &types, int capacity);
  void create_entity(const ComponentInitializerList &list);
  void update_systems();

  template<typename E>
  void send_event(const E &event)
  {
    for (EventDescription<E> *descr : core().events<E>())
      descr->eventHandler(event);
  }
  void free_ecs();


}
