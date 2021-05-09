#include "common_ui.inl"
//Code-generator production

void fps_ui_func();

static ecs::SystemDescription fps_ui_descr("fps_ui", {
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


void debug_console_ui_func();

static ecs::SystemDescription debug_console_ui_descr("debug_console_ui", {
  {ecs::get_type_description<string>("project"), false}
}, debug_console_ui_func, ecs::SystemOrder::UI);

void debug_console_ui_func()
{
  for (ecs::QueryIterator begin = debug_console_ui_descr.begin(), end = debug_console_ui_descr.end(); begin != end; ++begin)
  {
    debug_console_ui(
      *begin.get_component<string>(0)
    );
  }
}



