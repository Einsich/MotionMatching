#include "ui_render.inl"
//Code-generator production

void briefing_ui_func();

ecs::SystemDescription briefing_ui_descr("briefing_ui", {
}, briefing_ui_func, ecs::SystemOrder::UI);

void briefing_ui_func()
{
  for (ecs::QueryIterator begin = briefing_ui_descr.begin(), end = briefing_ui_descr.end(); begin != end; ++begin)
  {
    briefing_ui(
    );
  }
}


void motion_matching_statistic_func();

ecs::SystemDescription motion_matching_statistic_descr("motion_matching_statistic", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, motion_matching_statistic_func,  ecs::SystemOrder::UI);

void motion_matching_statistic_func()
{
  for (ecs::QueryIterator begin = motion_matching_statistic_descr.begin(), end = motion_matching_statistic_descr.end(); begin != end; ++begin)
  {
    motion_matching_statistic(
      *begin.get_component<AnimationPlayer>(0)
    );
  }
}


void menu_ui_func();

ecs::SystemDescription menu_ui_descr("menu_ui", {
}, menu_ui_func, ecs::SystemOrder::UI);

void menu_ui_func()
{
  for (ecs::QueryIterator begin = menu_ui_descr.begin(), end = menu_ui_descr.end(); begin != end; ++begin)
  {
    menu_ui(
    );
  }
}


void mm_early_text_perf_func();

ecs::SystemDescription mm_early_text_perf_descr("mm_early_text_perf", {
}, mm_early_text_perf_func, ecs::SystemOrder::UI);

void mm_early_text_perf_func()
{
  for (ecs::QueryIterator begin = mm_early_text_perf_descr.begin(), end = mm_early_text_perf_descr.end(); begin != end; ++begin)
  {
    mm_early_text_perf(
    );
  }
}



