#include "dagor_test.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache cache_trach__cache__;

static ecs::QueryCache dag_soa_update__cache__;

static ecs::QueryCache prune_cache___cache__;

static ecs::QueryCache dag_ecs_update__cache__;

static ecs::QueryCache prune_cache0__cache__;

static ecs::QueryCache dag_vector_structs_update__cache__;

static ecs::QueryCache prune_cache1__cache__;

static ecs::QueryCache dag_vector_pointers_update__cache__;

static ecs::QueryCache prune_cache2__cache__;

static ecs::QueryCache dag_vector_pointers_virtual_update__cache__;

static ecs::QueryCache prune_cache3__cache__;

static ecs::QueryCache dag_init__cache__;

static void cache_trach_implementation()
{
  ecs::perform_system(cache_trach__cache__, cache_trach);
}

static void dag_soa_update_implementation()
{
  ecs::perform_system(dag_soa_update__cache__, dag_soa_update);
}

static void prune_cache__implementation()
{
  ecs::perform_system(prune_cache___cache__, prune_cache_);
}

static void dag_ecs_update_implementation()
{
  ecs::perform_system(dag_ecs_update__cache__, dag_ecs_update);
}

static void prune_cache0_implementation()
{
  ecs::perform_system(prune_cache0__cache__, prune_cache0);
}

static void dag_vector_structs_update_implementation()
{
  ecs::perform_system(dag_vector_structs_update__cache__, dag_vector_structs_update);
}

static void prune_cache1_implementation()
{
  ecs::perform_system(prune_cache1__cache__, prune_cache1);
}

static void dag_vector_pointers_update_implementation()
{
  ecs::perform_system(dag_vector_pointers_update__cache__, dag_vector_pointers_update);
}

static void prune_cache2_implementation()
{
  ecs::perform_system(prune_cache2__cache__, prune_cache2);
}

static void dag_vector_pointers_virtual_update_implementation()
{
  ecs::perform_system(dag_vector_pointers_virtual_update__cache__, dag_vector_pointers_virtual_update);
}

static void prune_cache3_implementation()
{
  ecs::perform_system(prune_cache3__cache__, prune_cache3);
}

static void dag_init_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), dag_init__cache__, dag_init);
}

static void dag_init_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), dag_init__cache__, dag_init);
}

static void registration_pull_dagor_test()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "cache_trach",
  &cache_trach__cache__,
  {
    {"data0", ecs::get_type_index<mat4>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat4>()},
    {"data1", ecs::get_type_index<mat4>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat4>()},
    {"data2", ecs::get_type_index<mat4>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat4>()},
    {"data3", ecs::get_type_index<mat4>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat4>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &cache_trach_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "dag_soa_update",
  &dag_soa_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &dag_soa_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "prune_cache_",
  &prune_cache___cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &prune_cache__implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "dag_ecs_update",
  &dag_ecs_update__cache__,
  {
    {"p", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"v", ecs::get_type_index<vec3>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<vec3>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &dag_ecs_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "prune_cache0",
  &prune_cache0__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &prune_cache0_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "dag_vector_structs_update",
  &dag_vector_structs_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &dag_vector_structs_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "prune_cache1",
  &prune_cache1__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &prune_cache1_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "dag_vector_pointers_update",
  &dag_vector_pointers_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &dag_vector_pointers_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "prune_cache2",
  &prune_cache2__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &prune_cache2_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "dag_vector_pointers_virtual_update",
  &dag_vector_pointers_virtual_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &dag_vector_pointers_virtual_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "prune_cache3",
  &prune_cache3__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &prune_cache3_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "dag_init",
  &dag_init__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &dag_init_handler, &dag_init_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_dagor_test)
ECS_PULL_DEFINITION(variable_pull_dagor_test)
