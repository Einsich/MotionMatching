#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/system_description.h"
#include "manager/entity_id.h"
#include "template/blk_template.h"
namespace ecs
{
  struct Event;
  template<typename E>
  struct EventDescription;

  struct EntityContainer;
  std::vector<QueryDescription*> &all_queries();
  struct Core
  {
    std::unordered_map<uint, FullTypeDescription> types;
    std::vector<CallableDescription*> all_callable;
    std::vector<SystemDescription*> systems;
    std::vector<CallableDescription*> event_queries;
    EntityContainer *entityContainer;
    std::vector<std::function<void()>> events_cleaners;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    uint applicationTags;
    string currentSceneTags;
    std::string sceneToLoad;
    bool reloadScene;
    Core();
    ~Core();
    
    struct EventsCleaner
    {
      EventsCleaner(std::function<void()> &&cleaner, Core &core)
      {
        core.events_cleaners.emplace_back(std::move(cleaner));
      }
    };
    template<typename E>
    std::vector<EventDescription<E>*> &events_handler()
    {
      static std::vector<EventDescription<E>*> handlers;
      static EventsCleaner cleaner([&](){handlers.clear();}, *this);
      return handlers;
    }
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue(bool with_swap_last_element);
    void update_systems_subscribes();
    void register_allowed_callable();
  };
  Core &core();

  template<typename E>
  struct EventDescription final : CallableDescription
  {
    static_assert(std::is_base_of<Event, E>::value);
    typedef  void (*EventHandler)(const Event&);
    typedef  void (*SingleEventHandler)(const Event&, ecs::EntityId);
    EventHandler broadcastEventHandler;
    SingleEventHandler unicastEventHandler;
    EventDescription(const char *name, 
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      std::vector<std::string> &&before, std::vector<std::string> &&after,
      EventHandler broadcastEventHandler,
      SingleEventHandler unicastEventHandler,
      uint tags):
      CallableDescription(name, std::move(require_args), std::move(require_not_args),
        std::move(scenes), std::move(before), std::move(after), tags),
      broadcastEventHandler(broadcastEventHandler),
      unicastEventHandler(unicastEventHandler)
    {
      core().all_callable.push_back(this);
    }
    void registration() override
    {
      core().events_handler<E>().push_back(this);
      core().event_queries.push_back((CallableDescription*)this);
    }
  };

  template<typename T>
  struct ComponentInitializer
  {
    const char *name;
    const T & component;
  };
  template<typename T>
  uint component_initializer_hash(const ComponentInitializer<T> &component)
  {
    return register_type_description<T>(component.name);
  }
  template<typename T> 
  void component_copy(void *data, const T & component)
  {
    new (data) T(std::move(component));
  }
  template<int N, typename T, typename ...Args>
  void component_initializer_copy(vector<void*> &data, const ComponentInitializer<T> & arg, const Args &...args)
  {
    component_copy(data[N], arg.component);
    if constexpr (sizeof...(args) > 0)
      component_initializer_copy<N+1>(data, args...);
  }
  
  pair<EntityId, Archetype&> add_entity(const vector<uint> & type_hashes);
  template<typename ...Args>
  EntityId create_entity(const ComponentInitializer<Args> &...args)
  {
    vector<uint> typeHashes = 
      {register_type_description<EntityId>("eid"), component_initializer_hash(args)... };
    auto [eid, archetype] = add_entity(typeHashes);
    vector<void*> data = archetype.get_entity_data(typeHashes);
    component_copy(data[0], eid);
    component_initializer_copy<1>(data, args...);
    send_event_immediate(eid, OnEntityCreated());
    return eid;
  }
  struct Template;
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list);
  EntityId create_entity(const Template *temp);
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);

  //destoy current scene and load new scene from path
  void create_scene(const string &path, bool reload = true);


  template<typename E>
  void send_event(const E &event)
  {
    core().events.push([event](){
      for (EventDescription<E> *descr : core().events_handler<E>())
        descr->broadcastEventHandler(event);
    });
  }
  template<typename E>
  void send_event_immediate(const E &event)
  {
    for (EventDescription<E> *descr : core().events_handler<E>())
      descr->broadcastEventHandler(event);
  }

  template<typename E>
  void send_event(EntityId eid, const E &event)
  {
    if (eid)
    {
      core().events.push([eid, event](){
        for (EventDescription<E> *descr : core().events_handler<E>())
          descr->unicastEventHandler(event, eid);
      });
    }
  }
  template<typename E>
  void send_event_immediate(EntityId eid, const E &event)
  {
    if (eid)
    {
      for (EventDescription<E> *descr : core().events_handler<E>())
        descr->unicastEventHandler(event, eid);
    }
  }

  void system_statistic();
}
