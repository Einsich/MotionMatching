#pragma once
#include "manager/system_description.h"
#include "manager/entity_id.h"
#include "manager/entity_manager.h"
#include "template.h"
namespace ecs
{
  struct Event;
  struct EventDescription;

  extern EntityManager *entityManager;

  void set_entity_manager(EntityManager &manager);

  struct Template;
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list = {});
  EntityId create_entity(const char *template_name, ComponentInitializerList &&list = {});
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);


  template<typename T>
  void unicast_event(const ecs::Event &event, ecs::EntityId eid);
  template<typename T>
  void broadcast_event(const ecs::Event &event);

  template<typename E>
  void send_event(const E &event)
  {
    entityManager->send_event(event);
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
      entityManager->send_event(eid, event);
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
