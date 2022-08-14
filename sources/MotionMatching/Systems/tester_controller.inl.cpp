#include "tester_controller.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription get_tests_descr("get_tests", {
  {ecs::get_type_hash<ecs::vector<AnimationTest>>(), ecs::get_name_hash("tests"), false}
}, {
});

template<typename Callable>
void get_tests(Callable lambda)
{
  ecs::perform_query<ecs::vector<AnimationTest>&>
  (get_tests_descr, lambda);
}


ecs::QueryDescription get_tests2_descr("get_tests2", {
  {ecs::get_type_hash<ecs::vector<AnimationTest>>(), ecs::get_name_hash("tests"), false}
}, {
});

template<typename Callable>
void get_tests2(Callable lambda)
{
  ecs::perform_query<ecs::vector<AnimationTest>&>
  (get_tests2_descr, lambda);
}


void tester_update_func();

ecs::SystemDescription tester_update_descr("tester_update", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("testerSeed"), false},
  {ecs::get_type_hash<AnimationTester>(), ecs::get_name_hash("animationTester"), false}
}, {
},
{},
{},
tester_update_func, "act", {}, false);

void tester_update_func()
{
  ecs::perform_system(tester_update_descr, tester_update);
}

void test_count_func();

ecs::SystemDescription test_count_descr("test_count", {
  {ecs::get_type_hash<ecs::vector<ecs::EntityId>>(), ecs::get_name_hash("testers"), false},
  {ecs::get_type_hash<ecs::vector<AnimationTest>>(), ecs::get_name_hash("tests"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false}
}, {
},
{},
{},
test_count_func, "act", {"game"}, false);

void test_count_func()
{
  ecs::perform_system(test_count_descr, test_count);
}

void start_test_handler(const ecs::Event &event);
void start_test_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription start_test_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "start_test", {
  {ecs::get_type_hash<AnimationTester>(), ecs::get_name_hash("animationTester"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<PersonController>(), ecs::get_name_hash("personController"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("testerSeed"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false}
}, {
},
{},
{},
start_test_handler, start_test_singl_handler, {});

void start_test_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, start_test_descr, start_test);
}
void start_test_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, start_test_descr, eid, start_test);
}


