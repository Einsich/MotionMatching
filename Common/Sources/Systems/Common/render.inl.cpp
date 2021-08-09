#include "render.inl"
//Code-generator production

void add_global_uniform_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> add_global_uniform_descr("add_global_uniform", {
}, add_global_uniform_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = add_global_uniform_descr.begin(), end = add_global_uniform_descr.end(); begin != end; ++begin)
  {
    add_global_uniform(
      event
    );
  }
}


void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> add_global_uniform_singl_descr("add_global_uniform", {
}, add_global_uniform_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  add_global_uniform(
    event
  );
}



