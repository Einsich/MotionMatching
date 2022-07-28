#include "entity_manager.h"
#include "ecs_core.h"
#include "ecs_event.h"

namespace ecs
{
  void EntityManager::destroy_entities_from_destroy_queue()
  {
    for (int i = 0, n = toDestroy.size(); i < n; i++)
    {
      EntityId &eid = toDestroy.front();
      if (eid)
      {
        send_event_immediate(eid, OnEntityDestroyed());
        entityManager->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index());
        entityManager->entityPull.destroy_entity(eid);
      }
      toDestroy.pop();
    }
  }

  void system_sort(const ecs::vector<ecs::string> &applicationTags);
  void register_archetype(Archetype *archetype);
  void EntityManager::resolve_system_order_and_subscribes()
  {
    system_sort(applicationTags);
    for (auto &archetype : archetypes)
      register_archetype(archetype.get());
  }


  void EntityManager::process_events()
  {
    destroy_entities_from_destroy_queue();
    for (int i = 0, n = events.size(); i < n; i++)
    {
      events.front()();
      events.pop();
    }
  }

  void EntityManager::clear_scene()
  {
    process_events();
    for (auto it = entityPull.begin(), end = entityPull.end(); it != end; ++it)
    {
      EntityId eid = it.eid();
      if (eid)
      {
        send_event_immediate(eid, OnEntityDestroyed());
      }
    }
    while (!toDestroy.empty())
      toDestroy.pop();
    while (!events.empty())
      events.pop();
    for (auto &archetype : archetypes)
      archetype->clear();
    entityManager->entityPull.clear();
  }
}