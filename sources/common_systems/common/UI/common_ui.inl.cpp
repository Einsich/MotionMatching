#include "common_ui.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache fps_ui__cache__;

static ecs::QueryCache debug_console_ui__cache__;

static void fps_ui_implementation()
{
  ecs::perform_system(fps_ui__cache__, fps_ui);
}

static void debug_console_ui_implementation()
{
  ecs::perform_system(debug_console_ui__cache__, debug_console_ui);
}

static void registration_pull_common_ui()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "fps_ui",
  &fps_ui__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &fps_ui_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "debug_console_ui",
  &debug_console_ui__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {"debug"},
  &debug_console_ui_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_common_ui)
ECS_PULL_DEFINITION(variable_pull_common_ui)
