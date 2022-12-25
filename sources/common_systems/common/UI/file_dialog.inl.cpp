#include "file_dialog.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache open_dialog__cache__;

static void open_dialog_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiMenuRender &>(event), open_dialog__cache__, open_dialog);
}

static void open_dialog_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiMenuRender &>(event), open_dialog__cache__, open_dialog);
}

static void registration_pull_file_dialog()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "open_dialog",
  &open_dialog__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &open_dialog_handler, &open_dialog_single_handler),
  ecs::EventIndex<ImguiMenuRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_file_dialog)
