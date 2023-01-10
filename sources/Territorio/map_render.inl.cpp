#include "map_render.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache get_world_renderer__cache__;

static ecs::QueryCache gather_sprites__cache__;

static ecs::QueryCache render_scene__cache__;

static ecs::QueryCache update_map_textures__cache__;

template<typename Callable>
static void get_world_renderer(Callable lambda)
{
  ecs::perform_query<const WorldRenderer&>(get_world_renderer__cache__, lambda);
}

template<typename Callable>
static void gather_sprites(Callable lambda)
{
  ecs::perform_query<const Asset<Texture2D>&, const Asset<Texture2D>&, Shader&, ecs::vector<vec3>&, const vec4*>(gather_sprites__cache__, lambda);
}

static void render_scene_implementation()
{
  ecs::perform_system(render_scene__cache__, render_scene);
}

static void update_map_textures_implementation()
{
  ecs::perform_system(update_map_textures__cache__, update_map_textures);
}

static void registration_pull_map_render()
{
  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/map_render.inl:18",
  "get_world_renderer",
  &get_world_renderer__cache__,
  {
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<WorldRenderer>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/map_render.inl:35",
  "gather_sprites",
  &gather_sprites__cache__,
  {
    {"mapTexture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"borderTexture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"mapShader", ecs::get_type_index<Shader>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Shader>()},
    {"land_colors", ecs::get_type_index<ecs::vector<vec3>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec3>>()},
    {"color", ecs::get_type_index<vec4>(), ecs::AccessType::ReadOnly, true, ecs::is_singleton<vec4>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/map_render.inl:25",
  "render_scene",
  &render_scene__cache__,
  {
    {"cameraProjection", ecs::get_type_index<mat3>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<mat3>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<WorldRenderer>()}
  },
  {},
  {},
  "render",
  {},
  {},
  {},
  &render_scene_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/map_render.inl:58",
  "update_map_textures",
  &update_map_textures__cache__,
  {
    {"mapWasChanged", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()},
    {"mapTexture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MapArrays>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &update_map_textures_implementation));

}
ECS_FILE_REGISTRATION(&registration_pull_map_render)
ECS_PULL_DEFINITION(variable_pull_map_render)
