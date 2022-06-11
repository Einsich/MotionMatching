#include "test.inl"
#include <ecs_perform.h>
//Code-generator production

void ecs_update_func();

ecs::SystemDescription ecs_update_descr("ecs_update", {
  {ecs::get_type_description<vec3>("pos"), false},
  {ecs::get_type_description<vec3>("vel"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("m"), false}
}, {
}, {"game","editor"},
{},
{},
ecs_update_func, ecs::stage::act, ecs::tags::all, false);

void ecs_update_func()
{
  ecs::perform_system(ecs_update_descr, ecs_update);
}

void vector_structs_update_func();

ecs::SystemDescription vector_structs_update_descr("vector_structs_update", {
}, {
}, {"game","editor"},
{},
{},
vector_structs_update_func, ecs::stage::act, ecs::tags::all, false);

void vector_structs_update_func()
{
  ecs::perform_system(vector_structs_update_descr, vector_structs_update);
}

void vector_pointers_update_func();

ecs::SystemDescription vector_pointers_update_descr("vector_pointers_update", {
}, {
}, {"game","editor"},
{},
{},
vector_pointers_update_func, ecs::stage::act, ecs::tags::all, false);

void vector_pointers_update_func()
{
  ecs::perform_system(vector_pointers_update_descr, vector_pointers_update);
}

void init_handler(const ecs::Event &event);
void init_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_descr("init", {
}, {
}, {"game","editor"},
{},
{},
init_handler, init_singl_handler, ecs::tags::all);

void init_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_descr, init);
}
void init_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_descr, eid, init);
}


