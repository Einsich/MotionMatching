#include "profiler.inl"
//Code-generator production

void menu_profiler_func();

ecs::SystemDescription menu_profiler_descr("menu_profiler", {
}, menu_profiler_func, ecs::SystemOrder::UIMENU,  ecs::SystemTag::GameEditor);

void menu_profiler_func()
{
  for (ecs::QueryIterator begin = menu_profiler_descr.begin(), end = menu_profiler_descr.end(); begin != end; ++begin)
  {
    menu_profiler(
    );
  }
}



