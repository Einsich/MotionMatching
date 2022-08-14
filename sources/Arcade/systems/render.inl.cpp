#include "render.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_sprites_descr("gather_sprites", {
  {ecs::get_type_hash<Sprite>(), ecs::get_name_hash("sprite"), false},
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<vec4>(), ecs::get_name_hash("color"), true}
}, {
});

template<typename Callable>
void gather_sprites(Callable lambda)
{
  ecs::perform_query<Sprite&, Transform2D&, vec4*>
  (gather_sprites_descr, lambda);
}


void render_scene_func();

ecs::SystemDescription render_scene_descr("render_scene", {
  {ecs::get_type_hash<WorldRenderer>(), ecs::get_name_hash("wr"), false},
  {ecs::get_type_hash<mat3>(), ecs::get_name_hash("cameraProjection"), false},
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false}
}, {
},
{},
{},
render_scene_func, "render", {}, false);

void render_scene_func()
{
  ecs::perform_system(render_scene_descr, render_scene);
}


