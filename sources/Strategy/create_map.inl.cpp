#include "create_map.inl"
//Code-generator production

void scene_created_handler(const ecs::OnSceneCreated &event);
void scene_created_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> scene_created_descr("scene_created", {
}, scene_created_handler, scene_created_singl_handler, ecs::SystemTag::Game);

void scene_created_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, scene_created_descr, scene_created);
}
void scene_created_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, scene_created_descr, eid, scene_created);
}


