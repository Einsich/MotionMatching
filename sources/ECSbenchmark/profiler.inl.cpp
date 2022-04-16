#include "profiler.inl"
//Code-generator production

void menu_profiler_func();

ecs::SystemDescription menu_profiler_descr("menu_profiler", {
}, {
}, {},
menu_profiler_func, ecs::SystemOrder::UIMENU,  ecs::SystemTag::GameEditor,
{},
{});

void menu_profiler_func()
{
  ecs::perform_system(menu_profiler_descr, menu_profiler);
}


