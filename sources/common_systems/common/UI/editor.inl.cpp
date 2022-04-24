#include "editor.inl"
#include <ecs_perform.h>
//Code-generator production

void resources_menu_func();

ecs::SystemDescription resources_menu_descr("resources_menu", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false}
}, {
}, {"editor"},
{},
{},
resources_menu_func, ecs::stage::ui_menu, ecs::tags::all, false);

void resources_menu_func()
{
  ecs::perform_system(resources_menu_descr, resources_menu);
}

void asset_viewer_func();

ecs::SystemDescription asset_viewer_descr("asset_viewer", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false},
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"editor"},
{},
{},
asset_viewer_func, ecs::stage::ui, ecs::tags::all, false);

void asset_viewer_func()
{
  ecs::perform_system(asset_viewer_descr, asset_viewer);
}


