#include "editor_ui.inl"
//Code-generator production

void entity_viewer_func();

ecs::SystemDescription entity_viewer_descr("entity_viewer", {
}, entity_viewer_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::GameEditor));

void entity_viewer_func()
{
  for (ecs::QueryIterator begin = entity_viewer_descr.begin(), end = entity_viewer_descr.end(); begin != end; ++begin)
  {
    entity_viewer(
    );
  }
}


void ecs_types_viewer_func();

ecs::SystemDescription ecs_types_viewer_descr("ecs_types_viewer", {
}, ecs_types_viewer_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::Editor));

void ecs_types_viewer_func()
{
  for (ecs::QueryIterator begin = ecs_types_viewer_descr.begin(), end = ecs_types_viewer_descr.end(); begin != end; ++begin)
  {
    ecs_types_viewer(
    );
  }
}


void entity_creater_func();

ecs::SystemDescription entity_creater_descr("entity_creater", {
}, entity_creater_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::Editor));

void entity_creater_func()
{
  for (ecs::QueryIterator begin = entity_creater_descr.begin(), end = entity_creater_descr.end(); begin != end; ++begin)
  {
    entity_creater(
    );
  }
}



