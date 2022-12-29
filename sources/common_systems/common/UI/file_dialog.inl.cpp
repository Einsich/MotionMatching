#include "file_dialog.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache open_dialog__cache__;

static void open_dialog_implementation()
{
  ecs::perform_system(open_dialog__cache__, open_dialog);
}

static void registration_pull_file_dialog()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "open_dialog",
  &open_dialog__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  "imgui_menu",
  {},
  {},
  {"editor"},
  &open_dialog_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_file_dialog)
ECS_PULL_DEFINITION(variable_pull_file_dialog)
