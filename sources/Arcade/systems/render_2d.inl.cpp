#include "render_2d.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache gather_sprites__cache__;

static ecs::QueryCache render_scene__cache__;

template<typename Callable>
static void gather_sprites(Callable lambda)
{
  ecs::perform_query<const Sprite&, const Transform2D&, const vec4*>(gather_sprites__cache__, lambda);
}

static void render_scene_implementation()
{
  ecs::perform_system(render_scene__cache__, render_scene);
}

static void registration_pull_render_2d()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_sprites",
  &gather_sprites__cache__,
  {
    {"sprite", ecs::get_type_index<Sprite>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Sprite>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"color", ecs::get_type_index<vec4>(), ecs::AccessType::ReadOnly, true, ecs::is_singleton<vec4>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "render_scene",
  &render_scene__cache__,
  {
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<WorldRenderer>()},
    {"cameraProjection", ecs::get_type_index<mat3>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<mat3>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()}
  },
  {},
  {},
  "render",
  {},
  {},
  {},
  &render_scene_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_render_2d)
ECS_PULL_DEFINITION(variable_pull_render_2d)
