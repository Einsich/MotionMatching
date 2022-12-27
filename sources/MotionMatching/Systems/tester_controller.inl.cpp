#include "tester_controller.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache get_tests__cache__;

static ecs::QueryCache get_tests2__cache__;

static ecs::QueryCache tester_update__cache__;

static ecs::QueryCache test_count__cache__;

static ecs::QueryCache start_test__cache__;

template<typename Callable>
static void get_tests(Callable lambda)
{
  ecs::perform_query<const ecs::vector<AnimationTest>&>(get_tests__cache__, lambda);
}

template<typename Callable>
static void get_tests2(Callable lambda)
{
  ecs::perform_query<const ecs::vector<AnimationTest>&>(get_tests2__cache__, lambda);
}

static void tester_update_implementation()
{
  ecs::perform_system(tester_update__cache__, tester_update);
}

static void test_count_implementation()
{
  ecs::perform_system(test_count__cache__, test_count);
}

static void start_test_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), start_test__cache__, start_test);
}

static void start_test_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), start_test__cache__, start_test);
}

static void registration_pull_tester_controller()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "get_tests",
  &get_tests__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<AnimationTest>>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "get_tests2",
  &get_tests2__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<AnimationTest>>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "tester_update",
  &tester_update__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"testerSeed", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()},
    {"animationTester", ecs::get_type_index<AnimationTester>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationTester>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &tester_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "test_count",
  &test_count__cache__,
  {
    {"testers", ecs::get_type_index<ecs::vector<ecs::EntityId>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<ecs::EntityId>>()},
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {"game"},
  &test_count_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "start_test",
  &start_test__cache__,
  {
    {"animationTester", ecs::get_type_index<AnimationTester>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationTester>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()},
    {"testerSeed", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  {},
  {},
  {},
  &start_test_handler, &start_test_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_tester_controller)
ECS_PULL_DEFINITION(variable_pull_tester_controller)
