#include "editor.inl"
//Code-generator production

void resources_menu_func();

ecs::SystemDescription resources_menu_descr("resources_menu", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false}
}, resources_menu_func, ecs::SystemOrder::UIMENU, (uint)( ecs::SystemTag::Editor));

void resources_menu_func()
{
  for (ecs::QueryIterator begin = resources_menu_descr.begin(), end = resources_menu_descr.end(); begin != end; ++begin)
  {
    resources_menu(
      *begin.get_component<SelectedAsset>(0)
    );
  }
}


void asset_viewer_func();

ecs::SystemDescription asset_viewer_descr("asset_viewer", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false},
  {ecs::get_type_description<EditorUI>("ui"), false}
}, asset_viewer_func, ecs::SystemOrder::UI, (uint)( ecs::SystemTag::Editor));

void asset_viewer_func()
{
  for (ecs::QueryIterator begin = asset_viewer_descr.begin(), end = asset_viewer_descr.end(); begin != end; ++begin)
  {
    asset_viewer(
      *begin.get_component<SelectedAsset>(0),
      *begin.get_component<EditorUI>(1)
    );
  }
}



