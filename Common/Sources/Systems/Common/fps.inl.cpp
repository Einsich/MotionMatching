#include "fps.inl"
//Code-generator production

void fps_ui_func();

ecs::SystemDescription fps_ui_descr("fps_ui", {
  {ecs::get_type_description<float>("fps"), false}
}, fps_ui_func, ecs::SystemOrder::UI);

void fps_ui_func()
{
  for (ecs::QueryIterator begin = fps_ui_descr.begin(), end = fps_ui_descr.end(); begin != end; ++begin)
  {
    fps_ui(
      *begin.get_component<float>(0)
    );
  }
}



