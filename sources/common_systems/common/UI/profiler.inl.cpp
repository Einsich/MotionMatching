#include "profiler.inl"
#include <ecs_perform.h>
//Code-generator production

void menu_profiler_func();

ecs::SystemDescription menu_profiler_descr("menu_profiler", {
}, {
},
{},
{},
menu_profiler_func, "ui_menu", {}, false);

void menu_profiler_func()
{
  ecs::perform_system(menu_profiler_descr, menu_profiler);
}


