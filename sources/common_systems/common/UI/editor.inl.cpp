#include "editor.inl"
//Code-generator production

void resources_menu_func();

ecs::SystemDescription resources_menu_descr("resources_menu", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false}
}, resources_menu_func, ecs::SystemOrder::UIMENU,  ecs::SystemTag::Editor);

void resources_menu_func()
{
  ecs::perform_system(resources_menu_descr, resources_menu);
}

void asset_viewer_func();

ecs::SystemDescription asset_viewer_descr("asset_viewer", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false},
  {ecs::get_type_description<EditorUI>("ui"), false}
}, asset_viewer_func, ecs::SystemOrder::UI,  ecs::SystemTag::Editor);

void asset_viewer_func()
{
  ecs::perform_system(asset_viewer_descr, asset_viewer);
}


