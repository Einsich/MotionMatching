#include "create_map.inl"
#include <ecs_perform.h>
//Code-generator production

void scene_created_handler(const ecs::Event &event);
void scene_created_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription scene_created_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "scene_created", {
}, {
},
{},
{},
scene_created_handler, scene_created_singl_handler, {});

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

ecs::EventDescription spawn_buildings_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "spawn_buildings", {
  {ecs::get_type_hash<ecs::vector<ecs::string>>(), ecs::get_name_hash("items_templates"), false},
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("center"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("step_length"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("row_count"), false}
}, {
},
{},
{},
spawn_buildings_handler, spawn_buildings_singl_handler, {});

void spawn_buildings_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, spawn_buildings_descr, spawn_buildings);
}
void spawn_buildings_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, spawn_buildings_descr, eid, spawn_buildings);
}


