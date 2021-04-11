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
}, test_system_func, ecs::SystemOrder::INPUT_DEPEND);

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


void Lols_func();

ecs::SystemDescription Lols_descr({
  {ecs::get_type_description<std::string>("s"), false},
  {ecs::get_type_description<float>("f"), false}
}, Lols_func, ecs::SystemOrder::NO_ORDER);

void Lols_func()
{
  for (ecs::QueryIterator begin = Lols_descr.begin(), end = Lols_descr.end(); begin != end; ++begin)
  {
    Lols(
      *begin.get_component<std::string>(0),
      *begin.get_component<float>(1)
    );
  }
}


void math_system_func();

ecs::SystemDescription math_system_descr({
  {ecs::get_type_description<A>("v"), false},
  {ecs::get_type_description<B>("w"), true}
}, math_system_func, ecs::SystemOrder::SPECIAL);

void math_system_func()
{
  for (ecs::QueryIterator begin = math_system_descr.begin(), end = math_system_descr.end(); begin != end; ++begin)
  {
    math_system(
      *begin.get_component<A>(0),
       begin.get_component<B>(1)
    );
  }
}



