#include "file_dialog.inl"
#include <ecs_perform.h>
//Code-generator production

void open_dialog_func();

ecs::SystemDescription open_dialog_descr("open_dialog", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
},
{},
{},
open_dialog_func, "ui_menu", {"editor"}, false);

void open_dialog_func()
{
  ecs::perform_system(open_dialog_descr, open_dialog);
}


