#include "test.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache ecs_update__cache__;

static ecs::QueryCache vector_structs_update__cache__;

static ecs::QueryCache vector_pointers_update__cache__;

static ecs::QueryCache das_update__cache__;

static ecs::QueryCache init__cache__;

static ecs::QueryCache init_das__cache__;

static ecs::QueryCache term_das__cache__;

static void ecs_update_implementation()
{
  ecs::perform_system(ecs_update__cache__, ecs_update);
}

static void vector_structs_update_implementation()
{
  ecs::perform_system(vector_structs_update__cache__, vector_structs_update);
}

static void vector_pointers_update_implementation()
{
  ecs::perform_system(vector_pointers_update__cache__, vector_pointers_update);
}

static void das_update_implementation()
{
  ecs::perform_system(das_update__cache__, das_update);
}

static void init_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void init_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void init_das_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init_das__cache__, init_das);
}

static void init_das_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init_das__cache__, init_das);
}

static void term_das_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneTerminated &>(event), term_das__cache__, term_das);
}

static void term_das_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneTerminated &>(event), term_das__cache__, term_das);
}

static void registration_pull_test()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "ecs_update",
  &ecs_update__cache__,
  {
    {"pos", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"vel", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"center", ecs::get_type_index<vec3>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<vec3>()},
    {"m", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &ecs_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "vector_structs_update",
  &vector_structs_update__cache__,
  {},
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &vector_structs_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "vector_pointers_update",
  &vector_pointers_update__cache__,
  {},
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &vector_pointers_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "das_update",
  &das_update__cache__,
  {
    {"pos", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"vel", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"center", ecs::get_type_index<vec3>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<vec3>()},
    {"m", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &das_update_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "init",
  &init__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &init_handler, &init_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "init_das",
  &init_das__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &init_das_handler, &init_das_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "term_das",
  &term_das__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &term_das_handler, &term_das_single_handler),
  ecs::EventIndex<ecs::OnSceneTerminated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_test)
ECS_PULL_DEFINITION(variable_pull_test)
