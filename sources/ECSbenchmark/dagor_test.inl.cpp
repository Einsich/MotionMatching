#include "dagor_test.inl"
#include <ecs_perform.h>
//Code-generator production

void dag_ecs_update_func();

ecs::SystemDescription dag_ecs_update_descr("dag_ecs_update", {
  {ecs::get_type_description<vec3>("p"), false},
  {ecs::get_type_description<vec3>("v"), false}
}, {
}, {"game","editor"},
dag_ecs_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void dag_ecs_update_func()
{
  ecs::perform_system(dag_ecs_update_descr, dag_ecs_update);
}

void prune_cache0_func();

ecs::SystemDescription prune_cache0_descr("prune_cache0", {
}, {
}, {"game","editor"},
prune_cache0_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache0_func()
{
  ecs::perform_system(prune_cache0_descr, prune_cache0);
}

void dag_vector_structs_update_func();

ecs::SystemDescription dag_vector_structs_update_descr("dag_vector_structs_update", {
}, {
}, {"game","editor"},
dag_vector_structs_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void dag_vector_structs_update_func()
{
  ecs::perform_system(dag_vector_structs_update_descr, dag_vector_structs_update);
}

void prune_cache1_func();

ecs::SystemDescription prune_cache1_descr("prune_cache1", {
}, {
}, {"game","editor"},
prune_cache1_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache1_func()
{
  ecs::perform_system(prune_cache1_descr, prune_cache1);
}

void dag_vector_pointers_update_func();

ecs::SystemDescription dag_vector_pointers_update_descr("dag_vector_pointers_update", {
}, {
}, {"game","editor"},
dag_vector_pointers_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void dag_vector_pointers_update_func()
{
  ecs::perform_system(dag_vector_pointers_update_descr, dag_vector_pointers_update);
}

void prune_cache2_func();

ecs::SystemDescription prune_cache2_descr("prune_cache2", {
}, {
}, {"game","editor"},
prune_cache2_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache2_func()
{
  ecs::perform_system(prune_cache2_descr, prune_cache2);
}

void dag_vector_pointers_virtual_update_func();

ecs::SystemDescription dag_vector_pointers_virtual_update_descr("dag_vector_pointers_virtual_update", {
}, {
}, {"game","editor"},
dag_vector_pointers_virtual_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void dag_vector_pointers_virtual_update_func()
{
  ecs::perform_system(dag_vector_pointers_virtual_update_descr, dag_vector_pointers_virtual_update);
}

void prune_cache3_func();

ecs::SystemDescription prune_cache3_descr("prune_cache3", {
}, {
}, {"game","editor"},
prune_cache3_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache3_func()
{
  ecs::perform_system(prune_cache3_descr, prune_cache3);
}

void tiny_ecs_update_func();

ecs::SystemDescription tiny_ecs_update_descr("tiny_ecs_update", {
  {ecs::get_type_description<vec3>("P"), false},
  {ecs::get_type_description<vec3>("V"), false}
}, {
}, {"game","editor"},
tiny_ecs_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void tiny_ecs_update_func()
{
  ecs::perform_system(tiny_ecs_update_descr, tiny_ecs_update);
}

void prune_cache4_func();

ecs::SystemDescription prune_cache4_descr("prune_cache4", {
}, {
}, {"game","editor"},
prune_cache4_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache4_func()
{
  ecs::perform_system(prune_cache4_descr, prune_cache4);
}

void tiny_soa_structs_update_func();

ecs::SystemDescription tiny_soa_structs_update_descr("tiny_soa_structs_update", {
}, {
}, {"game","editor"},
tiny_soa_structs_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void tiny_soa_structs_update_func()
{
  ecs::perform_system(tiny_soa_structs_update_descr, tiny_soa_structs_update);
}

void prune_cache5_func();

ecs::SystemDescription prune_cache5_descr("prune_cache5", {
}, {
}, {"game","editor"},
prune_cache5_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache5_func()
{
  ecs::perform_system(prune_cache5_descr, prune_cache5);
}

void tiny_vector_structs_update_func();

ecs::SystemDescription tiny_vector_structs_update_descr("tiny_vector_structs_update", {
}, {
}, {"game","editor"},
tiny_vector_structs_update_func, ecs::stage::act, ecs::tags::all,
{},
{});

void tiny_vector_structs_update_func()
{
  ecs::perform_system(tiny_vector_structs_update_descr, tiny_vector_structs_update);
}

void prune_cache6_func();

ecs::SystemDescription prune_cache6_descr("prune_cache6", {
}, {
}, {"game","editor"},
prune_cache6_func, ecs::stage::act, ecs::tags::all,
{},
{});

void prune_cache6_func()
{
  ecs::perform_system(prune_cache6_descr, prune_cache6);
}

void dag_init_handler(const ecs::OnSceneCreated &event);
void dag_init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> dag_init_descr("dag_init", {
}, {
}, {"game","editor"},
dag_init_handler, dag_init_singl_handler, ecs::tags::all);

void dag_init_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, dag_init_descr, dag_init);
}
void dag_init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, dag_init_descr, eid, dag_init);
}


