#include "ray_tracing.inl"
//Code-generator production

void ray_tracing_func();

ecs::SystemDescription ray_tracing_descr({
  {ecs::get_type_description<PostFX>("rayTracing"), false},
  {ecs::get_type_description<CubeMap>("skyBox"), false},
  {ecs::get_type_description<Texture3D>("perlinNoise3D"), false}
}, ray_tracing_func, ecs::SystemOrder::LATE_RENDER);

void ray_tracing_func()
{
  for (ecs::QueryIterator begin = ray_tracing_descr.begin(), end = ray_tracing_descr.end(); begin != end; ++begin)
  {
    ray_tracing(
      *begin.get_component<PostFX>(0),
      *begin.get_component<CubeMap>(1),
      *begin.get_component<Texture3D>(2)
    );
  }
}


void ray_tracing_init_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> ray_tracing_init_descr({
  {ecs::get_type_description<PostFX>("rayTracing"), false},
  {ecs::get_type_description<CubeMap>("skyBox"), false},
  {ecs::get_type_description<Texture3D>("perlinNoise3D"), false}
}, ray_tracing_init_handler);

void ray_tracing_init_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = ray_tracing_init_descr.begin(), end = ray_tracing_init_descr.end(); begin != end; ++begin)
  {
    ray_tracing_init(
      event,
      *begin.get_component<PostFX>(0),
      *begin.get_component<CubeMap>(1),
      *begin.get_component<Texture3D>(2)
    );
  }
}


void ray_tracing_init_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> ray_tracing_init_singl_descr({
  {ecs::get_type_description<PostFX>("rayTracing"), false},
  {ecs::get_type_description<CubeMap>("skyBox"), false},
  {ecs::get_type_description<Texture3D>("perlinNoise3D"), false}
}, ray_tracing_init_singl_handler);

void ray_tracing_init_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  ray_tracing_init(
    event,
    *begin.get_component<PostFX>(0),
    *begin.get_component<CubeMap>(1),
    *begin.get_component<Texture3D>(2)
  );
}



