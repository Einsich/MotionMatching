#include "editor.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache resources_menu__cache__;

static ecs::QueryCache asset_viewer__cache__;

static void resources_menu_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiMenuRender &>(event), resources_menu__cache__, resources_menu);
}

static void resources_menu_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiMenuRender &>(event), resources_menu__cache__, resources_menu);
}

static void asset_viewer_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), asset_viewer__cache__, asset_viewer);
}

static void asset_viewer_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), asset_viewer__cache__, asset_viewer);
}

static void registration_pull_editor()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "resources_menu",
  &resources_menu__cache__,
  {
    {"selectedAsset", ecs::get_type_index<SelectedAsset>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SelectedAsset>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &resources_menu_handler, &resources_menu_single_handler),
  ecs::EventIndex<ImguiMenuRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "asset_viewer",
  &asset_viewer__cache__,
  {
    {"selectedAsset", ecs::get_type_index<SelectedAsset>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SelectedAsset>()},
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()},
    {"widgets", ecs::get_type_index<EditorWidgets>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorWidgets>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &asset_viewer_handler, &asset_viewer_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_editor)
