#include "editor.inl"
//Code-generator production

void resources_menu_func();

ecs::SystemDescription resources_menu_descr("resources_menu", {
  {ecs::get_type_description<SelectedAsset>("selectedAsset"), false}
}, resources_menu_func, ecs::SystemOrder::UIMENU, (uint)( ecs::SystemTag::GameEditor));

void resources_menu_func()
{
  for (ecs::QueryIterator begin = resources_menu_descr.begin(), end = resources_menu_descr.end(); begin != end; ++begin)
  {
    resources_menu(
      *begin.get_component<SelectedAsset>(0)
    );
  }
}



