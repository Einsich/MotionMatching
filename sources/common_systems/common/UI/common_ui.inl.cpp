#include "common_ui.inl"
#include <ecs_perform.h>
//Code-generator production

void fps_ui_func();

ecs::SystemDescription fps_ui_descr("fps_ui", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"editor","game"},
{},
{},
fps_ui_func, "ui", {}, false);

void fps_ui_func()
{
  ecs::perform_system(fps_ui_descr, fps_ui);
}

void debug_console_ui_func();

ecs::SystemDescription debug_console_ui_descr("debug_console_ui", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor"},
{},
{},
debug_console_ui_func, "ui", {"debug"}, false);

void debug_console_ui_func()
{
  ecs::perform_system(debug_console_ui_descr, debug_console_ui);
}


