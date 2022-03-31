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

void spawn_buildings_handler(const ecs::OnSceneCreated &event);
void spawn_buildings_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> spawn_buildings_descr("spawn_buildings", {
  {ecs::get_type_description<vector<string>>("items_templates"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("step_length"), false},
  {ecs::get_type_description<int>("row_count"), false}
}, spawn_buildings_handler, spawn_buildings_singl_handler, ecs::SystemTag::GameEditor);

void spawn_buildings_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, spawn_buildings_descr, spawn_buildings);
}
void spawn_buildings_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, spawn_buildings_descr, eid, spawn_buildings);
}


