#include "render.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_sprites_descr("gather_sprites", {
  {ecs::get_type_description<Sprite>("sprite"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec4>("color"), true}
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
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<Transform2D>("transform"), false}
}, {
},
{},
{},
render_scene_func, "render", {}, false);

void render_scene_func()
{
  ecs::perform_system(render_scene_descr, render_scene);
}


