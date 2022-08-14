#include "motion_matching.inl"
#include <ecs_perform.h>
//Code-generator production

void motion_matching_update_func();

ecs::SystemDescription motion_matching_update_descr("motion_matching_update", {
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<AnimationPlayer>(), ecs::get_name_hash("animationPlayer"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("material"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("mmIndex"), true},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("mmOptimisationIndex"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("updateMMStatistic"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false},
  {ecs::get_type_hash<SettingsContainer>(), ecs::get_name_hash("settingsContainer"), false},
  {ecs::get_type_hash<MMProfiler>(), ecs::get_name_hash("profiler"), false},
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false}
}, {
},
{"animation_player_update"},
{},
motion_matching_update_func, "act", {"game"}, false);

void motion_matching_update_func()
{
  ecs::perform_system(motion_matching_update_descr, motion_matching_update);
}


