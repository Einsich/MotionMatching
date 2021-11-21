#include "file_dialog.inl"
//Code-generator production

void open_dialog_func();

ecs::SystemDescription open_dialog_descr("open_dialog", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, open_dialog_func, ecs::SystemOrder::UIMENU,  ecs::SystemTag::Editor);

void open_dialog_func()
{
  ecs::perform_system(open_dialog_descr, open_dialog);
}


