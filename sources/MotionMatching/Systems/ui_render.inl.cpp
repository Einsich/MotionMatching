#include "ui_render.inl"
#include <ecs_perform.h>
//Code-generator production

void briefing_ui_func();

ecs::SystemDescription briefing_ui_descr("briefing_ui", {
}, {
},
{},
{},
briefing_ui_func, "ui", {}, false);

void briefing_ui_func()
{
  ecs::perform_system(briefing_ui_descr, briefing_ui);
}

void motion_matching_statistic_func();

ecs::SystemDescription motion_matching_statistic_descr("motion_matching_statistic", {
  {ecs::get_type_hash<AnimationPlayer>(), ecs::get_name_hash("animationPlayer"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("updateMMStatistic"), false},
  {-1u, ecs::get_name_hash("thirdPersonController"), false}
}, {
},
{},
{},
motion_matching_statistic_func, "ui", {}, false);

void motion_matching_statistic_func()
{
  ecs::perform_system(motion_matching_statistic_descr, motion_matching_statistic);
}

void current_anim_index_func();

ecs::SystemDescription current_anim_index_descr("current_anim_index", {
  {ecs::get_type_hash<AnimationPlayer>(), ecs::get_name_hash("animationPlayer"), false},
  {-1u, ecs::get_name_hash("thirdPersonController"), false}
}, {
},
{},
{},
current_anim_index_func, "ui", {}, false);

void current_anim_index_func()
{
  ecs::perform_system(current_anim_index_descr, current_anim_index);
}

void menu_ui_func();

ecs::SystemDescription menu_ui_descr("menu_ui", {
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false},
  {ecs::get_type_hash<SettingsContainer>(), ecs::get_name_hash("settingsContainer"), false}
}, {
},
{},
{},
menu_ui_func, "ui_menu", {}, false);

void menu_ui_func()
{
  ecs::perform_system(menu_ui_descr, menu_ui);
}

void mm_early_text_perf_func();

ecs::SystemDescription mm_early_text_perf_descr("mm_early_text_perf", {
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false}
}, {
},
{},
{},
mm_early_text_perf_func, "ui", {}, false);

void mm_early_text_perf_func()
{
  ecs::perform_system(mm_early_text_perf_descr, mm_early_text_perf);
}


