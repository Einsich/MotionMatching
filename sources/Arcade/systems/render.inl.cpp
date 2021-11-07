#include "render.inl"
//Code-generator production

ecs::QueryDescription gather_sprites_descr("gather_sprites", {
  {ecs::get_type_description<Sprite>("sprite"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec4>("color"), true}
});

template<typename Callable>
void gather_sprites(Callable lambda)
{
  for (ecs::QueryIterator begin = gather_sprites_descr.begin(), end = gather_sprites_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<Sprite>(0),
      *begin.get_component<Transform2D>(1),
       begin.get_component<vec4>(2)
    );
  }
}


void render_scene_func();

ecs::SystemDescription render_scene_descr("render_scene", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<Transform2D>("transform"), false}
}, render_scene_func, ecs::SystemOrder::RENDER, ecs::SystemTag::Game);

void render_scene_func()
{
  for (ecs::QueryIterator begin = render_scene_descr.begin(), end = render_scene_descr.end(); begin != end; ++begin)
  {
    render_scene(
      *begin.get_component<WorldRenderer>(0),
      *begin.get_component<mat3>(1),
      *begin.get_component<Transform2D>(2)
    );
  }
}



