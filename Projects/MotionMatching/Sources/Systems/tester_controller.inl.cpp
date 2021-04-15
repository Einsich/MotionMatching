#include "tester_controller.inl"
//Code-generator production

void tester_update_func();

ecs::SystemDescription tester_update_descr("tester_update", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<PersonController>("personController"), false}
}, tester_update_func, ecs::SystemOrder::LOGIC);

void tester_update_func()
{
  for (ecs::QueryIterator begin = tester_update_descr.begin(), end = tester_update_descr.end(); begin != end; ++begin)
  {
    tester_update(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<AnimationTester>(1),
      *begin.get_component<AnimationPlayer>(2),
      *begin.get_component<Transform>(3),
      *begin.get_component<PersonController>(4)
    );
  }
}


void start_test_handler(const OnAnimationTestStart &event);

ecs::EventDescription<OnAnimationTestStart> start_test_descr("start_test", {
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<PersonController>("personController"), false}
}, start_test_handler);

void start_test_handler(const OnAnimationTestStart &event)
{
  for (ecs::QueryIterator begin = start_test_descr.begin(), end = start_test_descr.end(); begin != end; ++begin)
  {
    start_test(
      event,
      *begin.get_component<AnimationTester>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<PersonController>(2)
    );
  }
}


void start_test_singl_handler(const OnAnimationTestStart &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<OnAnimationTestStart> start_test_singl_descr("start_test", {
  {ecs::get_type_description<AnimationTester>("animationTester"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<PersonController>("personController"), false}
}, start_test_singl_handler);

void start_test_singl_handler(const OnAnimationTestStart &event, ecs::QueryIterator &begin)
{
  start_test(
    event,
    *begin.get_component<AnimationTester>(0),
    *begin.get_component<Transform>(1),
    *begin.get_component<PersonController>(2)
  );
}



