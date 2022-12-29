#include "physics_systems.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache gather_all_target_colliders__cache__;

static ecs::QueryCache move_all_entity_with_velocity__cache__;

static ecs::QueryCache collision_detection__cache__;

template<typename Callable>
static void gather_all_target_colliders(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, const Transform2D&, bool&>(gather_all_target_colliders__cache__, lambda);
}

static void move_all_entity_with_velocity_implementation()
{
  ecs::perform_system(move_all_entity_with_velocity__cache__, move_all_entity_with_velocity);
}

static void collision_detection_implementation()
{
  ecs::perform_system(collision_detection__cache__, collision_detection);
}

static void registration_pull_physics_systems()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_all_target_colliders",
  &gather_all_target_colliders__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"destroyed", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"target", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "move_all_entity_with_velocity",
  &move_all_entity_with_velocity__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"velocity", ecs::get_type_index<vec2>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<vec2>()},
    {"rotationVelocity", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, true, ecs::is_singleton<float>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &move_all_entity_with_velocity_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "collision_detection",
  &collision_detection__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()}
  },
  {
    {"bullet", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  "act",
  {},
  {},
  {},
  &collision_detection_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_physics_systems)
ECS_PULL_DEFINITION(variable_pull_physics_systems)
