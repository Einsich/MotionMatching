#include "create_map.inl"
//Code-generator production

void scene_created_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> scene_created_descr("scene_created", {
}, scene_created_handler);

void scene_created_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = scene_created_descr.begin(), end = scene_created_descr.end(); begin != end; ++begin)
  {
    scene_created(
      event
    );
  }
}


void scene_created_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> scene_created_singl_descr("scene_created", {
}, scene_created_singl_handler);

void scene_created_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  scene_created(
    event
  );
}



