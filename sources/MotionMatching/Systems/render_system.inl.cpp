#include "render_system.inl"
#include <ecs_perform.h>
//Code-generator production

void process_animation_func();

ecs::SystemDescription process_animation_descr("process_animation", {
  {ecs::get_type_hash<Asset<Mesh>>(), ecs::get_name_hash("mesh"), false},
  {ecs::get_type_hash<AnimationPlayer>(), ecs::get_name_hash("animationPlayer"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<ecs::vector<mat3x4>>(), ecs::get_name_hash("bones_matrices"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false}
}, {
},
{},
{},
process_animation_func, "render", {}, false);

void process_animation_func()
{
  ecs::perform_system(process_animation_descr, process_animation);
}


