#include "editor.inl"
#include <ecs_perform.h>
//Code-generator production

void resources_menu_func();

ecs::SystemDescription resources_menu_descr("resources_menu", {
  {ecs::get_type_hash<SelectedAsset>(), ecs::get_name_hash("selectedAsset"), false}
}, {
},
{},
{},
resources_menu_func, "ui_menu", {"editor"}, false);

void resources_menu_func()
{
  ecs::perform_system(resources_menu_descr, resources_menu);
}

void asset_viewer_func();

ecs::SystemDescription asset_viewer_descr("asset_viewer", {
  {ecs::get_type_hash<SelectedAsset>(), ecs::get_name_hash("selectedAsset"), false},
  {ecs::get_type_hash<EditorUI>(), ecs::get_name_hash("ui"), false},
  {ecs::get_type_hash<EditorWidgets>(), ecs::get_name_hash("widgets"), false}
}, {
},
{},
{},
asset_viewer_func, "ui", {"editor"}, false);

void asset_viewer_func()
{
  ecs::perform_system(asset_viewer_descr, asset_viewer);
}


