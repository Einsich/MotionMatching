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


void entity_created_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> entity_created_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, entity_created_handler);

void entity_created_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = entity_created_descr.begin(), end = entity_created_descr.end(); begin != end; ++begin)
  {
    entity_created(
      event,
      *begin.get_component<std::string>(0)
    );
  }
}


void on_scene_create_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> on_scene_create_singl_descr({
}, on_scene_create_singl_handler);

void on_scene_create_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  on_scene_create(
    event
  );
}


void entity_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> entity_created_singl_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, entity_created_singl_handler);

void entity_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  entity_created(
    event,
    *begin.get_component<std::string>(0)
  );
}



