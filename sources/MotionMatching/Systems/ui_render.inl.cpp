#include "ui_render.inl"
//Code-generator production

void briefing_ui_func();

ecs::SystemDescription briefing_ui_descr("briefing_ui", {
}, briefing_ui_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void briefing_ui_func()
{
  ecs::perform_system(briefing_ui_descr, briefing_ui);
}

void motion_matching_statistic_func();

ecs::SystemDescription motion_matching_statistic_descr("motion_matching_statistic", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<bool>("updateMMStatistic"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, motion_matching_statistic_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void motion_matching_statistic_func()
{
  ecs::perform_system(motion_matching_statistic_descr, motion_matching_statistic);
}

void current_anim_index_func();

ecs::SystemDescription current_anim_index_descr("current_anim_index", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, current_anim_index_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void current_anim_index_func()
{
  ecs::perform_system(current_anim_index_descr, current_anim_index);
}

void menu_ui_func();

ecs::SystemDescription menu_ui_descr("menu_ui", {
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, menu_ui_func, ecs::SystemOrder::UIMENU, ecs::SystemTag::Game,
{},
{});

void menu_ui_func()
{
  ecs::perform_system(menu_ui_descr, menu_ui);
}

void mm_early_text_perf_func();

ecs::SystemDescription mm_early_text_perf_descr("mm_early_text_perf", {
  {ecs::get_type_description<Settings>("settings"), false}
}, mm_early_text_perf_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void mm_early_text_perf_func()
{
  ecs::perform_system(mm_early_text_perf_descr, mm_early_text_perf);
}


