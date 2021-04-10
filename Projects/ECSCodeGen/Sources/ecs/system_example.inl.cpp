#include "system_example.inl"
//Code-generator production

void test_system_func();

ecs::SystemDescription test_system_descr({
  {ecs::get_type_description<int>("a"), false},
  {ecs::get_type_description<float>("b"), false}
}, test_system_func);

void test_system_func()
{
  for (ecs::SystemIterator begin = test_system_descr.begin(), end = test_system_descr.end(); begin != end; ++begin)
  {
    test_system(
      *begin.get_component<int>(0),
      *begin.get_component<float>(1)
    );
  }
}


void Lol_system_func();

ecs::SystemDescription Lol_system_descr({
  {ecs::get_type_description<std::string>("s"), false},
  {ecs::get_type_description<float>("f"), false}
}, Lol_system_func);

void Lol_system_func()
{
  for (ecs::SystemIterator begin = Lol_system_descr.begin(), end = Lol_system_descr.end(); begin != end; ++begin)
  {
    Lol_system(
      *begin.get_component<std::string>(0),
      *begin.get_component<float>(1)
    );
  }
}


void math_system_func();

ecs::SystemDescription math_system_descr({
  {ecs::get_type_description<A>("v"), false},
  {ecs::get_type_description<B>("w"), true}
}, math_system_func);

void math_system_func()
{
  for (ecs::SystemIterator begin = math_system_descr.begin(), end = math_system_descr.end(); begin != end; ++begin)
  {
    math_system(
      *begin.get_component<A>(0),
       begin.get_component<B>(1)
    );
  }
}



