#include "start_scene.inl"
//Code-generator production

void scene_destroy_handler(const ecs::OnEntityDestroyed &event);

ecs::EventDescription<ecs::OnEntityDestroyed> scene_destroy_descr("scene_destroy", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false}
}, scene_destroy_handler, (uint)(ecs::SystemTag::Game));

void scene_destroy_handler(const ecs::OnEntityDestroyed &event)
{
  for (ecs::QueryIterator begin = scene_destroy_descr.begin(), end = scene_destroy_descr.end(); begin != end; ++begin)
  {
    scene_destroy(
      event,
      *begin.get_component<MotionMatchingScene>(0)
    );
  }
}


void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityDestroyed> scene_destroy_singl_descr("scene_destroy", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false}
}, scene_destroy_singl_handler, (uint)(ecs::SystemTag::Game));

void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin)
{
  scene_destroy(
    event,
      *begin.get_component<MotionMatchingScene>(0)
  );
}



