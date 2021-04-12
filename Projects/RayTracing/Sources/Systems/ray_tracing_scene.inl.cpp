#include "ray_tracing_scene.inl"
//Code-generator production

void init_scene_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_scene_descr({
}, init_scene_handler);

void init_scene_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = init_scene_descr.begin(), end = init_scene_descr.end(); begin != end; ++begin)
  {
    init_scene(
      event
    );
  }
}


void init_scene_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_scene_singl_descr({
}, init_scene_singl_handler);

void init_scene_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  init_scene(
    event
  );
}



