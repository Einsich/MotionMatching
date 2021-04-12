#include "test_init.inl"
//Code-generator production

ecs::QueryDescription query_instead_for_descr({
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<vec3>("p"), false},
  {ecs::get_type_description<vec3>("v"), false},
  {ecs::get_type_description<float>("m"), false}
});

template<typename Callable>
void query_instead_for(Callable lambda)
{
  for (ecs::QueryIterator begin = query_instead_for_descr.begin(), end = query_instead_for_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<vec3>(1),
      *begin.get_component<vec3>(2),
      *begin.get_component<float>(3)
    );
  }
}


ecs::SingleQueryDescription attsh_query_test_descr({
  {ecs::get_type_description<A>("v"), false},
  {ecs::get_type_description<B>("w"), false}
});

template<typename Callable>
void attsh_query_test(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(attsh_query_test_descr, eid, begin))
  {
    lambda(
      *begin.get_component<A>(0),
      *begin.get_component<B>(1)
    );
  }
}


void N_Body_func();

ecs::SystemDescription N_Body_descr({
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<vec3>("p"), false},
  {ecs::get_type_description<vec3>("v"), false},
  {ecs::get_type_description<float>("m"), false}
}, N_Body_func, ecs::SystemOrder::NO_ORDER);

void N_Body_func()
{
  for (ecs::QueryIterator begin = N_Body_descr.begin(), end = N_Body_descr.end(); begin != end; ++begin)
  {
    N_Body(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<vec3>(1),
      *begin.get_component<vec3>(2),
      *begin.get_component<float>(3)
    );
  }
}


void attach_test_func();

ecs::SystemDescription attach_test_descr({
  {ecs::get_type_description<ecs::EntityId>("attached_to"), false}
}, attach_test_func, ecs::SystemOrder::NO_ORDER);

void attach_test_func()
{
  for (ecs::QueryIterator begin = attach_test_descr.begin(), end = attach_test_descr.end(); begin != end; ++begin)
  {
    attach_test(
      *begin.get_component<ecs::EntityId>(0)
    );
  }
}


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


void entity_destroyed_handler(const ecs::OnEntityDestroyed &event);

ecs::EventDescription<ecs::OnEntityDestroyed> entity_destroyed_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, entity_destroyed_handler);

void entity_destroyed_handler(const ecs::OnEntityDestroyed &event)
{
  for (ecs::QueryIterator begin = entity_destroyed_descr.begin(), end = entity_destroyed_descr.end(); begin != end; ++begin)
  {
    entity_destroyed(
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


void entity_destroyed_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityDestroyed> entity_destroyed_singl_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, entity_destroyed_singl_handler);

void entity_destroyed_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin)
{
  entity_destroyed(
    event,
    *begin.get_component<std::string>(0)
  );
}


