#include "tester_controller.inl"
//Code-generator production

void tester_update_func();

ecs::SystemDescription tester_update_descr("tester_update", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, tester_update_func, ecs::SystemOrder::LOGIC, (uint)(ecs::SystemTag::Game));

void tester_update_func()
{
  for (ecs::QueryIterator begin = tester_update_descr.begin(), end = tester_update_descr.end(); begin != end; ++begin)
  {
    tester_update(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<AnimationTester>(1),
      *begin.get_component<AnimationPlayer>(2)
    );
  }
}


void test_count_func();

ecs::SystemDescription test_count_descr("test_count", {
  {ecs::get_type_description<vector<ecs::EntityId>>("testers"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, test_count_func, ecs::SystemOrder::NO_ORDER, (uint)(ecs::SystemTag::Game));

void test_count_func()
{
  for (ecs::QueryIterator begin = test_count_descr.begin(), end = test_count_descr.end(); begin != end; ++begin)
  {
    test_count(
      *begin.get_component<vector<ecs::EntityId>>(0),
      *begin.get_component<Settings>(1)
    );
  }
}


void start_test_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> start_test_descr("start_test", {
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, start_test_handler, (uint)(ecs::SystemTag::GameEditor));

void start_test_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = start_test_descr.begin(), end = start_test_descr.end(); begin != end; ++begin)
  {
    start_test(
      event,
      *begin.get_component<AnimationTester>(0),
      *begin.get_component<AnimationPlayer>(1),
      *begin.get_component<Transform>(2),
      *begin.get_component<PersonController>(3),
      *begin.get_component<Settings>(4)
    );
  }
}


void start_test_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> start_test_singl_descr("start_test", {
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, start_test_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void start_test_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  start_test(
    event,
      *begin.get_component<AnimationTester>(0),
      *begin.get_component<AnimationPlayer>(1),
      *begin.get_component<Transform>(2),
      *begin.get_component<PersonController>(3),
      *begin.get_component<Settings>(4)
  );
}



