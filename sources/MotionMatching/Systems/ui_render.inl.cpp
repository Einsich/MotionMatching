#include "ui_render.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache briefing_ui__cache__;

static ecs::QueryCache motion_matching_statistic__cache__;

static ecs::QueryCache current_anim_index__cache__;

static ecs::QueryCache menu_ui__cache__;

static ecs::QueryCache mm_early_text_perf__cache__;

static void briefing_ui_implementation()
{
  ecs::perform_system(briefing_ui__cache__, briefing_ui);
}

static void motion_matching_statistic_implementation()
{
  ecs::perform_system(motion_matching_statistic__cache__, motion_matching_statistic);
}

static void current_anim_index_implementation()
{
  ecs::perform_system(current_anim_index__cache__, current_anim_index);
}

static void menu_ui_implementation()
{
  ecs::perform_system(menu_ui__cache__, menu_ui);
}

static void mm_early_text_perf_implementation()
{
  ecs::perform_system(mm_early_text_perf__cache__, mm_early_text_perf);
}

static void registration_pull_ui_render()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/ui_render.inl:103",
  "briefing_ui",
  &briefing_ui__cache__,
  {},
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &briefing_ui_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/ui_render.inl:117",
  "motion_matching_statistic",
  &motion_matching_statistic__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<AnimationPlayer>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()},
    {"updateMMStatistic", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  "imgui_render",
  {},
  {},
  {},
  &motion_matching_statistic_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/ui_render.inl:145",
  "current_anim_index",
  &current_anim_index__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<AnimationPlayer>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  "imgui_render",
  {},
  {},
  {},
  &current_anim_index_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/ui_render.inl:242",
  "menu_ui",
  &menu_ui__cache__,
  {
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SettingsContainer>()}
  },
  {},
  {},
  "imgui_menu",
  {},
  {},
  {},
  &menu_ui_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/ui_render.inl:253",
  "mm_early_text_perf",
  &mm_early_text_perf__cache__,
  {
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &mm_early_text_perf_implementation);

}
ECS_FILE_REGISTRATION(&registration_pull_ui_render)
ECS_PULL_DEFINITION(variable_pull_ui_render)
