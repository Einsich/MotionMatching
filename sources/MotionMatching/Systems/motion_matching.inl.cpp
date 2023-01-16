#include "motion_matching.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache motion_matching_update__cache__;

static void motion_matching_update_implementation()
{
  ecs::perform_system(motion_matching_update__cache__, motion_matching_update);
}

static void registration_pull_motion_matching()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/motion_matching.inl:91",
  "motion_matching_update",
  &motion_matching_update__cache__,
  {
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()},
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"mmIndex", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, true, ecs::is_singleton<int>()},
    {"mmOptimisationIndex", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()},
    {"updateMMStatistic", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SettingsContainer>()},
    {"profiler", ecs::get_type_index<MMProfiler>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MMProfiler>()},
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()}
  },
  {},
  {},
  "act",
  {"animation_player_update"},
  {},
  {"game"},
  &motion_matching_update_implementation);

}
ECS_FILE_REGISTRATION(&registration_pull_motion_matching)
ECS_PULL_DEFINITION(variable_pull_motion_matching)
