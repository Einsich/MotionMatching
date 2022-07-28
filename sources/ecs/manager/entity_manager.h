#pragma once
#include "entity_pull.h"
#include "manager/entity_id.h"
#include "manager/component_instance.h"
#include "config/config.h"
#include "ecs_event.h"
#include "template.h"
#include <memory>
#include <queue>
#include <functional>
namespace ecs
{
  class Archetype;
  template<typename T>
  void unicast_event(const ecs::Event &event, ecs::EntityId eid);
  template<typename T>
  void broadcast_event(const ecs::Event &event);
  struct EntityManager
  {
    ecs::vector<ecs::string> applicationTags;
    ecs::vector<std::unique_ptr<Archetype>> archetypes;
    EntityPull entityPull;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    EntityManager(ecs::vector<ecs::string> &&applicationTags):
    applicationTags(std::move(applicationTags))
    {}
    EntityManager(const ecs::vector<ecs::string> &applicationTags):
    applicationTags(applicationTags)
    {}
    void process_events();
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue();
    void resolve_system_order_and_subscribes();

    void clear_scene();


    EntityId create_entity(const Template *temp, ComponentInitializerList &&list = {});
    EntityId create_entity(const char *template_name, ComponentInitializerList &&list = {});
    EntityId find_entity(uint archetype, uint index);
    void destroy_entity(const EntityId &eid);

    template<typename E>
    void send_event(const E &event)
    {
      events.push([event](){ broadcast_event<E>(event); });
    }

    template<typename E>
    void send_event(EntityId eid, const E &event)
    {
      if (eid)
      {
        events.push([event, eid](){ unicast_event<E>(event, eid); });
      }
    }
  };
}