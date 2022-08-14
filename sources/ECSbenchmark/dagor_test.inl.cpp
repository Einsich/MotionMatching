#include "dagor_test.inl"
#include <ecs_perform.h>
//Code-generator production

void cache_trach_func();

ecs::SystemDescription cache_trach_descr("cache_trach", {
  {ecs::get_type_hash<mat4>(), ecs::get_name_hash("data0"), false},
  {ecs::get_type_hash<mat4>(), ecs::get_name_hash("data1"), false},
  {ecs::get_type_hash<mat4>(), ecs::get_name_hash("data2"), false},
  {ecs::get_type_hash<mat4>(), ecs::get_name_hash("data3"), false}
}, {
},
{},
{},
cache_trach_func, "act", {}, false);

void cache_trach_func()
{
  ecs::perform_system(cache_trach_descr, cache_trach);
}

void dag_soa_update_func();

ecs::SystemDescription dag_soa_update_descr("dag_soa_update", {
}, {
},
{},
{},
dag_soa_update_func, "act", {}, false);

void dag_soa_update_func()
{
  ecs::perform_system(dag_soa_update_descr, dag_soa_update);
}

void prune_cache__func();

ecs::SystemDescription prune_cache__descr("prune_cache_", {
}, {
},
{},
{},
prune_cache__func, "act", {}, false);

void prune_cache__func()
{
  ecs::perform_system(prune_cache__descr, prune_cache_);
}

void dag_ecs_update_func();

ecs::SystemDescription dag_ecs_update_descr("dag_ecs_update", {
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("p"), false},
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("v"), false}
}, {
},
{},
{},
dag_ecs_update_func, "act", {}, false);

void dag_ecs_update_func()
{
  ecs::perform_system(dag_ecs_update_descr, dag_ecs_update);
}

void prune_cache0_func();

ecs::SystemDescription prune_cache0_descr("prune_cache0", {
}, {
},
{},
{},
prune_cache0_func, "act", {}, false);

void prune_cache0_func()
{
  ecs::perform_system(prune_cache0_descr, prune_cache0);
}

void dag_vector_structs_update_func();

ecs::SystemDescription dag_vector_structs_update_descr("dag_vector_structs_update", {
}, {
},
{},
{},
dag_vector_structs_update_func, "act", {}, false);

void dag_vector_structs_update_func()
{
  ecs::perform_system(dag_vector_structs_update_descr, dag_vector_structs_update);
}

void prune_cache1_func();

ecs::SystemDescription prune_cache1_descr("prune_cache1", {
}, {
},
{},
{},
prune_cache1_func, "act", {}, false);

void prune_cache1_func()
{
  ecs::perform_system(prune_cache1_descr, prune_cache1);
}

void dag_vector_pointers_update_func();

ecs::SystemDescription dag_vector_pointers_update_descr("dag_vector_pointers_update", {
}, {
},
{},
{},
dag_vector_pointers_update_func, "act", {}, false);

void dag_vector_pointers_update_func()
{
  ecs::perform_system(dag_vector_pointers_update_descr, dag_vector_pointers_update);
}

void prune_cache2_func();

ecs::SystemDescription prune_cache2_descr("prune_cache2", {
}, {
},
{},
{},
prune_cache2_func, "act", {}, false);

void prune_cache2_func()
{
  ecs::perform_system(prune_cache2_descr, prune_cache2);
}

void dag_vector_pointers_virtual_update_func();

ecs::SystemDescription dag_vector_pointers_virtual_update_descr("dag_vector_pointers_virtual_update", {
}, {
},
{},
{},
dag_vector_pointers_virtual_update_func, "act", {}, false);

void dag_vector_pointers_virtual_update_func()
{
  ecs::perform_system(dag_vector_pointers_virtual_update_descr, dag_vector_pointers_virtual_update);
}

void prune_cache3_func();

ecs::SystemDescription prune_cache3_descr("prune_cache3", {
}, {
},
{},
{},
prune_cache3_func, "act", {}, false);

void prune_cache3_func()
{
  ecs::perform_system(prune_cache3_descr, prune_cache3);
}

void dag_init_handler(const ecs::Event &event);
void dag_init_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription dag_init_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "dag_init", {
}, {
},
{},
{},
dag_init_handler, dag_init_singl_handler, {});

void dag_init_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, dag_init_descr, dag_init);
}
void dag_init_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, dag_init_descr, eid, dag_init);
}


