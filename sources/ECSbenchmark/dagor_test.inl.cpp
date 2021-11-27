#include "dagor_test.inl"
//Code-generator production

void dag_ecs_update_func();

ecs::SystemDescription dag_ecs_update_descr("dag_ecs_update", {
  {ecs::get_type_description<vec3>("p"), false},
  {ecs::get_type_description<vec3>("v"), false}
}, dag_ecs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void dag_ecs_update_func()
{
  ecs::perform_system(dag_ecs_update_descr, dag_ecs_update);
}

void prune_cache0_func();

ecs::SystemDescription prune_cache0_descr("prune_cache0", {
}, prune_cache0_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void prune_cache0_func()
{
  ecs::perform_system(prune_cache0_descr, prune_cache0);
}

void dag_vector_structs_update_func();

ecs::SystemDescription dag_vector_structs_update_descr("dag_vector_structs_update", {
}, dag_vector_structs_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void dag_vector_structs_update_func()
{
  ecs::perform_system(dag_vector_structs_update_descr, dag_vector_structs_update);
}

void prune_cache1_func();

ecs::SystemDescription prune_cache1_descr("prune_cache1", {
}, prune_cache1_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void prune_cache1_func()
{
  ecs::perform_system(prune_cache1_descr, prune_cache1);
}

void dag_vector_pointers_update_func();

ecs::SystemDescription dag_vector_pointers_update_descr("dag_vector_pointers_update", {
}, dag_vector_pointers_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void dag_vector_pointers_update_func()
{
  ecs::perform_system(dag_vector_pointers_update_descr, dag_vector_pointers_update);
}

void prune_cache2_func();

ecs::SystemDescription prune_cache2_descr("prune_cache2", {
}, prune_cache2_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void prune_cache2_func()
{
  ecs::perform_system(prune_cache2_descr, prune_cache2);
}

void dag_vector_pointers_virtual_update_func();

ecs::SystemDescription dag_vector_pointers_virtual_update_descr("dag_vector_pointers_virtual_update", {
}, dag_vector_pointers_virtual_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void dag_vector_pointers_virtual_update_func()
{
  ecs::perform_system(dag_vector_pointers_virtual_update_descr, dag_vector_pointers_virtual_update);
}

void prune_cache3_func();

ecs::SystemDescription prune_cache3_descr("prune_cache3", {
}, prune_cache3_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void prune_cache3_func()
{
  ecs::perform_system(prune_cache3_descr, prune_cache3);
}

void dag_init_handler(const ecs::OnSceneCreated &event);
void dag_init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> dag_init_descr("dag_init", {
}, dag_init_handler, dag_init_singl_handler, ecs::SystemTag::GameEditor);

void dag_init_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, dag_init_descr, dag_init);
}
void dag_init_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, dag_init_descr, eid, dag_init);
}


