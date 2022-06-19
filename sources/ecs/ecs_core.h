#pragma once
#include <queue>
#include <functional>
#include "manager/system_description.h"
#include "manager/entity_id.h"
#include "template.h"
namespace ecs
{
  struct Event;
  struct EventDescription;

  struct EntityContainer;
  ecs::vector<QueryDescription*> &all_queries();
  struct Core
  {
    EntityContainer *entityContainer;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    uint applicationTags;
    ecs::string currentSceneTags;
    ecs::string sceneToLoad;
    bool reloadScene;
    Core();
    ~Core();
    
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue(bool with_swap_last_element);
    void update_systems_subscribes();
    void register_allowed_callable();
  };
  Core &core();

  struct Template;
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list = {});
  EntityId create_entity(const char *template_name, ComponentInitializerList &&list = {});
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);

  //destoy current scene and load new scene from path
  void create_scene(const char *path, bool reload = true);


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
  void print_archetypes();
}
