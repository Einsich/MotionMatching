#include "common_ui.inl"
//Code-generator production

void fps_ui_func();

ecs::SystemDescription fps_ui_descr("fps_ui", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {},
fps_ui_func, ecs::SystemOrder::UI, ecs::SystemTag::Editor|ecs::SystemTag::Game,
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
}, {},
debug_console_ui_func, ecs::SystemOrder::UI, ecs::SystemTag::GameEditor|ecs::SystemTag::Debug,
{},
{});

void debug_console_ui_func()
{
  ecs::perform_system(debug_console_ui_descr, debug_console_ui);
}


