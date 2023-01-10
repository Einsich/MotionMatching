#include "create_map.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache scene_created__cache__;

static ecs::QueryCache spawn_buildings__cache__;

static void scene_created_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), scene_created__cache__, scene_created);
}

static void scene_created_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), scene_created__cache__, scene_created);
}

static void spawn_buildings_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), spawn_buildings__cache__, spawn_buildings);
}

static void spawn_buildings_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), spawn_buildings__cache__, spawn_buildings);
}

static void registration_pull_create_map()
{
  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/create_map.inl:47",
  "scene_created",
  &scene_created__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &scene_created_handler, &scene_created_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/create_map.inl:66",
  "spawn_buildings",
  &spawn_buildings__cache__,
  {
    {"items_templates", ecs::get_type_index<ecs::vector<ecs::string>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<ecs::string>>()},
    {"center", ecs::get_type_index<vec3>(), ecs::AccessType::Copy, false, ecs::is_singleton<vec3>()},
    {"step_length", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"row_count", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()}
  },
  {},
  {},
  {},
  {},
  {},
  &spawn_buildings_handler, &spawn_buildings_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_create_map)
ECS_PULL_DEFINITION(variable_pull_create_map)
