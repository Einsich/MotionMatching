#include "ui_render.inl"
//Code-generator production

void ui_render_func();

ecs::SystemDescription ui_render_descr("ui_render", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<bool>("showMatchingStatistic"), false}
}, ui_render_func, ecs::SystemOrder::UI);

void ui_render_func()
{
  for (ecs::QueryIterator begin = ui_render_descr.begin(), end = ui_render_descr.end(); begin != end; ++begin)
  {
    ui_render(
      *begin.get_component<AnimationPlayer>(0),
      *begin.get_component<bool>(1)
    );
  }
}



