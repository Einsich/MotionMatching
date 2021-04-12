#include "test_system.inl"
//Code-generator production

ecs::QueryDescription b_plus_f_query_descr({
  {ecs::get_type_description<float>("b"), false}
});

template<typename Callable>
void b_plus_f_query(Callable lambda)
{
  for (ecs::QueryIterator begin = b_plus_f_query_descr.begin(), end = b_plus_f_query_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<float>(0)
    );
  }
}

void test_system_func();

ecs::SystemDescription test_system_descr({
  {ecs::get_type_description<int>("a"), false},
  {ecs::get_type_description<float>("b"), false}
}, test_system_func, ecs::SystemOrder::LATE_RENDER);

void test_system_func()
{
  for (ecs::QueryIterator begin = test_system_descr.begin(), end = test_system_descr.end(); begin != end; ++begin)
  {
    test_system(
      *begin.get_component<int>(0),
      *begin.get_component<float>(1)
    );
  }
}


void system_with_query_func();

ecs::SystemDescription system_with_query_descr({
  {ecs::get_type_description<float>("f"), false},
  {ecs::get_type_description<std::string>("s"), false}
}, system_with_query_func, ecs::SystemOrder::NO_ORDER);

void system_with_query_func()
{
  for (ecs::QueryIterator begin = system_with_query_descr.begin(), end = system_with_query_descr.end(); begin != end; ++begin)
  {
    system_with_query(
      *begin.get_component<float>(0),
      *begin.get_component<std::string>(1)
    );
  }
}

void nullable_system_func();

ecs::SystemDescription nullable_system_descr({
  {ecs::get_type_description<A>("v"), false},
  {ecs::get_type_description<B>("w"), true}
}, nullable_system_func, ecs::SystemOrder::EARLY_RENDER);

void nullable_system_func()
{
  for (ecs::QueryIterator begin = nullable_system_descr.begin(), end = nullable_system_descr.end(); begin != end; ++begin)
  {
    nullable_system(
      *begin.get_component<A>(0),
       begin.get_component<B>(1)
    );
  }
}


void event_sender_func();

ecs::SystemDescription event_sender_descr({
  {ecs::get_type_description<float>("b"), false}
}, event_sender_func, ecs::SystemOrder::LATE_RENDER);

void event_sender_func()
{
  for (ecs::QueryIterator begin = event_sender_descr.begin(), end = event_sender_descr.end(); begin != end; ++begin)
  {
    event_sender(
      *begin.get_component<float>(0)
    );
  }
}


void eid_test_func();

ecs::SystemDescription eid_test_descr({
  {ecs::get_type_description<ecs::EntityId>("eid"), false}
}, eid_test_func, ecs::SystemOrder::NO_ORDER);

void eid_test_func()
{
  for (ecs::QueryIterator begin = eid_test_descr.begin(), end = eid_test_descr.end(); begin != end; ++begin)
  {
    eid_test(
      *begin.get_component<ecs::EntityId>(0)
    );
  }
}


void test_handler_handler(const TestEvent &event);

ecs::EventDescription<TestEvent> test_handler_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, test_handler_handler);

void test_handler_handler(const TestEvent &event)
{
  for (ecs::QueryIterator begin = test_handler_descr.begin(), end = test_handler_descr.end(); begin != end; ++begin)
  {
    test_handler(
      event,
      *begin.get_component<std::string>(0)
    );
  }
}


void test_handler_singl_handler(const TestEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<TestEvent> test_handler_singl_descr({
  {ecs::get_type_description<std::string>("s"), false}
}, test_handler_singl_handler);

void test_handler_singl_handler(const TestEvent &event, ecs::QueryIterator &begin)
{
  test_handler(
    event,
    *begin.get_component<std::string>(0)
  );
}



