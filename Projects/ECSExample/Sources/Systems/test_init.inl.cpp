#include "test_init.inl"
//Code-generator production

void on_scene_create_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> on_scene_create_descr({
}, on_scene_create_handler);

void on_scene_create_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = on_scene_create_descr.begin(), end = on_scene_create_descr.end(); begin != end; ++begin)
  {
    on_scene_create(
      event
    );
  }
}



