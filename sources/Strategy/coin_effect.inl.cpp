#include "coin_effect.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache coin_move__cache__;

static ecs::QueryCache spawn_coin_effect__cache__;

static void coin_move_implementation()
{
  ecs::perform_system(coin_move__cache__, coin_move);
}

static void spawn_coin_effect_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), spawn_coin_effect__cache__, spawn_coin_effect);
}

static void spawn_coin_effect_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), spawn_coin_effect__cache__, spawn_coin_effect);
}

static void registration_pull_coin_effect()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "coin_move",
  &coin_move__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"linear_velocity", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"angular_velocity", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"life_time", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<float>()},
    {"life_period", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {
    {"coinEffect", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &coin_move_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "spawn_coin_effect",
  &spawn_coin_effect__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()},
    {"heightMap", ecs::get_type_index<HeightMap>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<HeightMap>()}
  },
  {},
  {},
  {},
  {},
  {},
  &spawn_coin_effect_handler, &spawn_coin_effect_single_handler),
  ecs::EventIndex<MouseButtonDownEvent<MouseButton::LeftButton>>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_coin_effect)
ECS_PULL_DEFINITION(variable_pull_coin_effect)
