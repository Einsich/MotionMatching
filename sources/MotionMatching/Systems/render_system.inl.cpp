#include "render_system.inl"
//Code-generator production

void process_animation_func();

ecs::SystemDescription process_animation_descr("process_animation", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, process_animation_func, ecs::SystemOrder::RENDER, ecs::SystemTag::GameEditor);

void process_animation_func()
{
  for (ecs::QueryIterator begin = process_animation_descr.begin(), end = process_animation_descr.end(); begin != end; ++begin)
  {
    process_animation(
      *begin.get_component<Asset<Mesh>>(0),
      *begin.get_component<AnimationPlayer>(1),
      *begin.get_component<Transform>(2),
      *begin.get_component<Settings>(3)
    );
  }
}



