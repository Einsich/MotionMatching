#include "render_system.inl"
//Code-generator production

void process_animation_func();

ecs::SystemDescription process_animation_descr("process_animation", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, {
}, {"game","editor"},
process_animation_func, ecs::stage::render, ecs::tags::all,
{},
{});

void process_animation_func()
{
  ecs::perform_system(process_animation_descr, process_animation);
}


