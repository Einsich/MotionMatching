#include "test.inl"
//Code-generator production

void ecs_update_func();

ecs::SystemDescription ecs_update_descr("ecs_update", {
  {ecs::get_type_description<vec3>("pos"), false},
  {ecs::get_type_description<vec3>("vel"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("m"), false}
}, ecs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor,
{},
{});

void ecs_update_func()
{
  ecs::perform_system(ecs_update_descr, ecs_update);
}

void vector_structs_update_func();

ecs::SystemDescription vector_structs_update_descr("vector_structs_update", {
}, vector_structs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor,
{},
{});

void vector_structs_update_func()
{
  ecs::perform_system(vector_structs_update_descr, vector_structs_update);
}

void vector_pointers_update_func();

ecs::SystemDescription vector_pointers_update_descr("vector_pointers_update", {
}, vector_pointers_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor,
{},
{});

void vector_pointers_update_func()
{
  ecs::perform_system(vector_pointers_update_descr, vector_pointers_update);
}

void init_handler(const ecs::OnSceneCreated &event);
void init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_descr("init", {
}, init_handler, init_singl_handler, ecs::SystemTag::GameEditor);

void init_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_descr, init);
}
void init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_descr, eid, init);
}


