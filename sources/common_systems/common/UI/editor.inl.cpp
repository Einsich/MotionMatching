#include "editor.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache resources_menu__cache__;

static ecs::QueryCache asset_viewer__cache__;

static void resources_menu_implementation()
{
  ecs::perform_system(resources_menu__cache__, resources_menu);
}

static void asset_viewer_implementation()
{
  ecs::perform_system(asset_viewer__cache__, asset_viewer);
}

static void registration_pull_editor()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/UI/editor.inl:29",
  "resources_menu",
  &resources_menu__cache__,
  {
    {"selectedAsset", ecs::get_type_index<SelectedAsset>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SelectedAsset>()}
  },
  {},
  {},
  "imgui_menu",
  {},
  {},
  {"editor"},
  &resources_menu_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/UI/editor.inl:49",
  "asset_viewer",
  &asset_viewer__cache__,
  {
    {"selectedAsset", ecs::get_type_index<SelectedAsset>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SelectedAsset>()},
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {"editor"},
  &asset_viewer_implementation);

}
ECS_FILE_REGISTRATION(&registration_pull_editor)
ECS_PULL_DEFINITION(variable_pull_editor)
