#include "common_ui.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache fps_ui__cache__;

static ecs::QueryCache debug_console_ui__cache__;

static void fps_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), fps_ui__cache__, fps_ui);
}

static void fps_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), fps_ui__cache__, fps_ui);
}

static void debug_console_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), debug_console_ui__cache__, debug_console_ui);
}

static void debug_console_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), debug_console_ui__cache__, debug_console_ui);
}

static void registration_pull_common_ui()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "fps_ui",
  &fps_ui__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  {},
  {},
  {},
  &fps_ui_handler, &fps_ui_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "debug_console_ui",
  &debug_console_ui__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  {},
  {},
  {"debug"},
  &debug_console_ui_handler, &debug_console_ui_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_common_ui)
ECS_PULL_DEFINITION(variable_pull_common_ui)
