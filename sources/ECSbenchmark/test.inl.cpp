#include "test.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache ecs_update__cache__;

static ecs::QueryCache vector_structs_update__cache__;

static ecs::QueryCache vector_pointers_update__cache__;

static ecs::QueryCache test_event_send__cache__;

static ecs::QueryCache test_request_send__cache__;

static ecs::QueryCache init__cache__;

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

static void test_event_send_implementation()
{
  ecs::perform_system(test_event_send__cache__, test_event_send);
}

static void test_request_send_implementation()
{
  ecs::perform_system(test_request_send__cache__, test_request_send);
}

static void init_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void init_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void registration_pull_test()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:104",
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
  "act",
  {},
  {},
  {},
  &ecs_update_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:109",
  "vector_structs_update",
  &vector_structs_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &vector_structs_update_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:117",
  "vector_pointers_update",
  &vector_pointers_update__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &vector_pointers_update_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:144",
  "test_event_send",
  &test_event_send__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &test_event_send_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:159",
  "test_request_send",
  &test_request_send__cache__,
  {},
  {},
  {},
  "act",
  {},
  {},
  {},
  &test_request_send_implementation);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSbenchmark/test.inl:25",
  "init",
  &init__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &init_handler, &init_single_handler,
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_test)
ECS_PULL_DEFINITION(variable_pull_test)
