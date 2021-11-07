#include "file_dialog.inl"
//Code-generator production

void open_dialog_func();

ecs::SystemDescription open_dialog_descr("open_dialog", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, open_dialog_func, ecs::SystemOrder::UIMENU,  ecs::SystemTag::Editor);

void open_dialog_func()
{
  for (ecs::QueryIterator begin = open_dialog_descr.begin(), end = open_dialog_descr.end(); begin != end; ++begin)
  {
    open_dialog(
      *begin.get_component<EditorUI>(0)
    );
  }
}



