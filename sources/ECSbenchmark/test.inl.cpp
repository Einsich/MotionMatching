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
ecs_update_func, "act", {}, false);

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
vector_structs_update_func, "act", {}, false);

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
vector_pointers_update_func, "act", {}, false);

void vector_pointers_update_func()
{
  ecs::perform_system(vector_pointers_update_descr, vector_pointers_update);
}

void das_update_func();

ecs::SystemDescription das_update_descr("das_update", {
  {ecs::get_type_description<vec3>("pos"), false},
  {ecs::get_type_description<vec3>("vel"), false},
  {ecs::get_type_description<vec3>("center"), false},
  {ecs::get_type_description<float>("m"), false}
}, {
}, {"game","editor"},
{},
{},
das_update_func, "act", {}, false);

void das_update_func()
{
  ecs::perform_system(das_update_descr, das_update);
}

void init_handler(const ecs::Event &event);
void init_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "init", {
}, {
}, {"game","editor"},
{},
{},
init_handler, init_singl_handler, {});

void init_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_descr, init);
}
void init_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_descr, eid, init);
}

void init_das_handler(const ecs::Event &event);
void init_das_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_das_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "init_das", {
}, {
}, {"game","editor"},
{},
{},
init_das_handler, init_das_singl_handler, {});

void init_das_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_das_descr, init_das);
}
void init_das_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_das_descr, eid, init_das);
}

void term_das_handler(const ecs::Event &event);
void term_das_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription term_das_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneDestroy>(), "term_das", {
}, {
}, {"game","editor"},
{},
{},
term_das_handler, term_das_singl_handler, {});

void term_das_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneDestroy&)event, term_das_descr, term_das);
}
void term_das_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneDestroy&)event, term_das_descr, eid, term_das);
}


