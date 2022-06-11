#include "create_map.inl"
#include <ecs_perform.h>
//Code-generator production

void scene_created_handler(const ecs::Event &event);
void scene_created_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> scene_created_descr("scene_created", {
}, {
}, {},
{},
{},
scene_created_handler, scene_created_singl_handler, ecs::tags::all);

void scene_created_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, scene_created_descr, scene_created);
}
void scene_created_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, scene_created_descr, eid, scene_created);
}

void spawn_buildings_handler(const ecs::Event &event);
void spawn_buildings_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> spawn_buildings_descr("spawn_buildings", {
  {ecs::get_type_description<vector<string>>("items_templates"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("step_length"), false},
  {ecs::get_type_description<int>("row_count"), false}
}, {
}, {"game","editor"},
{},
{},
spawn_buildings_handler, spawn_buildings_singl_handler, ecs::tags::all);

void spawn_buildings_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, spawn_buildings_descr, spawn_buildings);
}
void spawn_buildings_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, spawn_buildings_descr, eid, spawn_buildings);
}


