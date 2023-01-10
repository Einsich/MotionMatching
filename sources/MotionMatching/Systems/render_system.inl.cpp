#include "render_system.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache process_animation__cache__;

static void process_animation_implementation()
{
  ecs::perform_system(process_animation__cache__, process_animation);
}

static void registration_pull_render_system()
{
  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/render_system.inl:10",
  "process_animation",
  &process_animation__cache__,
  {
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Mesh>>()},
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<AnimationPlayer>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform>()},
    {"bones_matrices", ecs::get_type_index<ecs::vector<mat3x4>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<mat3x4>>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  "render",
  {},
  {"lod_selector"},
  {},
  &process_animation_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_render_system)
ECS_PULL_DEFINITION(variable_pull_render_system)
