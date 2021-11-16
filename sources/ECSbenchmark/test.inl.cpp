#include "test.inl"
//Code-generator production

void ecs_update_func();

ecs::SystemDescription ecs_update_descr("ecs_update", {
  {ecs::get_type_description<vec3>("pos"), false},
  {ecs::get_type_description<vec3>("vel"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("m"), false}
}, ecs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void ecs_update_func()
{
  for (ecs::QueryIterator begin = ecs_update_descr.begin(), end = ecs_update_descr.end(); begin != end; ++begin)
  {
    ecs_update(
      *begin.get_component<vec3, 0>(),
      *begin.get_component<vec3, 1>(),
      *begin.get_component<vec3, 2>(),
      *begin.get_component<float, 3>()
    );
  }
}


void vector_structs_update_func();

ecs::SystemDescription vector_structs_update_descr("vector_structs_update", {
}, vector_structs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void vector_structs_update_func()
{
  for (ecs::QueryIterator begin = vector_structs_update_descr.begin(), end = vector_structs_update_descr.end(); begin != end; ++begin)
  {
    vector_structs_update(
    );
  }
}


void vector_pointers_update_func();

ecs::SystemDescription vector_pointers_update_descr("vector_pointers_update", {
}, vector_pointers_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void vector_pointers_update_func()
{
  for (ecs::QueryIterator begin = vector_pointers_update_descr.begin(), end = vector_pointers_update_descr.end(); begin != end; ++begin)
  {
    vector_pointers_update(
    );
  }
}


void init_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_descr("init", {
}, init_handler, ecs::SystemTag::GameEditor);

void init_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = init_descr.begin(), end = init_descr.end(); begin != end; ++begin)
  {
    init(
      event
    );
  }
}


void init_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_singl_descr("init", {
}, init_singl_handler, ecs::SystemTag::GameEditor);

void init_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  init(
    event
  );
}



