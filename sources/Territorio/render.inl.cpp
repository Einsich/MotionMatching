#include "render.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription get_world_renderer_descr("get_world_renderer", {
  {ecs::get_type_hash<WorldRenderer>(), ecs::get_name_hash("wr"), false}
}, {
});

template<typename Callable>
void get_world_renderer(Callable lambda)
{
  ecs::perform_query<WorldRenderer&>
  (get_world_renderer_descr, lambda);
}


ecs::QueryDescription gather_sprites_descr("gather_sprites", {
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("mapTexture"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("borderTexture"), false},
  {ecs::get_type_hash<Shader>(), ecs::get_name_hash("mapShader"), false},
  {ecs::get_type_hash<ecs::vector<vec3>>(), ecs::get_name_hash("land_colors"), false},
  {ecs::get_type_hash<vec4>(), ecs::get_name_hash("color"), true}
}, {
});

template<typename Callable>
void gather_sprites(Callable lambda)
{
  ecs::perform_query<Asset<Texture2D>&, Asset<Texture2D>&, Shader&, ecs::vector<vec3>&, vec4*>
  (gather_sprites_descr, lambda);
}


void render_scene_func();

ecs::SystemDescription render_scene_descr("render_scene", {
  {ecs::get_type_hash<mat3>(), ecs::get_name_hash("cameraProjection"), false},
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<WorldRenderer>(), ecs::get_name_hash("wr"), false}
}, {
},
{},
{},
render_scene_func, "render", {}, false);

void render_scene_func()
{
  ecs::perform_system(render_scene_descr, render_scene);
}

void update_map_textures_func();

ecs::SystemDescription update_map_textures_descr("update_map_textures", {
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("mapWasChanged"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("mapTexture"), false},
  {ecs::get_type_hash<MapArrays>(), ecs::get_name_hash("map_arrays"), false}
}, {
},
{},
{},
update_map_textures_func, "act", {}, false);

void update_map_textures_func()
{
  ecs::perform_system(update_map_textures_descr, update_map_textures);
}


