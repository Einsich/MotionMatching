#include "common_ui.inl"
//Code-generator production

void fps_ui_func();

ecs::SystemDescription fps_ui_descr("fps_ui", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"editor","scene","game"},
fps_ui_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void fps_ui_func()
{
  ecs::perform_system(fps_ui_descr, fps_ui);
}

void debug_console_ui_func();

ecs::SystemDescription debug_console_ui_descr("debug_console_ui", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor","scene","debug"},
debug_console_ui_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void debug_console_ui_func()
{
  ecs::perform_system(debug_console_ui_descr, debug_console_ui);
}


