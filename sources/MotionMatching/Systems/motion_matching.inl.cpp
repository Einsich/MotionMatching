#include "motion_matching.inl"
#include <ecs_perform.h>
//Code-generator production

void motion_matching_update_func();

ecs::SystemDescription motion_matching_update_descr("motion_matching_update", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<int>("mmIndex"), true},
  {ecs::get_type_description<int>("mmOptimisationIndex"), false},
  {ecs::get_type_description<bool>("updateMMStatistic"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false},
  {ecs::get_type_description<MMProfiler>("profiler"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
},
{"animation_player_update"},
{},
motion_matching_update_func, "act", {}, false);

void motion_matching_update_func()
{
  ecs::perform_system(motion_matching_update_descr, motion_matching_update);
}


