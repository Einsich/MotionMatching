#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/system_description.h"
#include "manager/entity_id.h"
#include "template/template.h"
namespace ecs
{
  struct Event;
  struct EventDescription;

  struct EntityContainer;
  std::vector<QueryDescription*> &all_queries();
  struct Core
  {
    std::unordered_map<uint, FullTypeDescription> types;
    EntityContainer *entityContainer;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    uint applicationTags;
    string currentSceneTags;
    std::string sceneToLoad;
    bool reloadScene;
    Core();
    ~Core();
    
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue(bool with_swap_last_element);
    void update_systems_subscribes();
    void register_allowed_callable();
  };
  Core &core();



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
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list = {});
  EntityId create_entity(const char *template_name, ComponentInitializerList &&list = {});
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);

  //destoy current scene and load new scene from path
  void create_scene(const string &path, bool reload = true);


  template<typename T>
  void unicast_event(const ecs::Event &event, ecs::EntityId eid);
  template<typename T>
  void broadcast_event(const ecs::Event &event);

  template<typename E>
  void send_event(const E &event)
  {
    core().events.push([event](){ broadcast_event<E>(event); });
  }
  template<typename E>
  void send_event_immediate(const E &event)
  {
    broadcast_event<E>(event);
  }

  template<typename E>
  void send_event(EntityId eid, const E &event)
  {
    if (eid)
    {
      core().events.push([event, eid](){ unicast_event<E>(event, eid); });
    }
  }
  template<typename E>
  void send_event_immediate(EntityId eid, const E &event)
  {
    if (eid)
    {
      unicast_event<E>(event, eid);
    }
  }

  void system_statistic();
}
