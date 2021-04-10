#include "system_example.inl"

void test_system_func();
void Lol_system_func();
void math_system_func();
ecs::SystemDescription test_system_descr({
  {ecs::get_type_description<int>("a"), false},
  {ecs::get_type_description<float>("b"), false}
  }, test_system_func);
ecs::SystemDescription Lol_system_descr({
  {ecs::get_type_description<std::string>("s"), false},
  {ecs::get_type_description<float>("f"), false}
  }, Lol_system_func);
ecs::SystemDescription math_system_descr({
  {ecs::get_type_description<A>("v"), false},
  {ecs::get_type_description<B>("w"), true}
  }, math_system_func);

void test_system_func()
{
  for (ecs::SystemCashedArchetype archetype : test_system_descr.archetypes)
  {
    for (int i = 0; i < archetype.archetype->componentCount; ++i)
    {
      test_system(
        *archetype.containers[0]->get_component<int>(i),
        *archetype.containers[1]->get_component<float>(i)
      );
    }
  }
}
void Lol_system_func()
{
  for (ecs::SystemCashedArchetype archetype : Lol_system_descr.archetypes)
  {
    for (int i = 0; i < archetype.archetype->componentCount; ++i)
    {
      Lol_system(
        *archetype.containers[0]->get_component<std::string>(i),
        *archetype.containers[1]->get_component<float>(i)
      );
    }
  }
}
void math_system_func()
{
  for (ecs::SystemCashedArchetype archetype : math_system_descr.archetypes)
  {
    for (int i = 0; i < archetype.archetype->componentCount; ++i)
    {
      math_system(
        *archetype.containers[0]->get_component<A>(i),
         archetype.containers[1]->get_component<B>(i)
      );
    }
  }
}