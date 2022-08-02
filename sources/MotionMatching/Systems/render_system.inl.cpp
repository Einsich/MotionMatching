#include "render_system.inl"
#include <ecs_perform.h>
//Code-generator production

void process_animation_func();

ecs::SystemDescription process_animation_descr("process_animation", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::vector<mat3x4>>("bones_matrices"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, {
},
{},
{},
process_animation_func, "render", {}, false);

void process_animation_func()
{
  ecs::perform_system(process_animation_descr, process_animation);
}


