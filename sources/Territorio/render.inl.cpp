#include "render.inl"
//Code-generator production

ecs::QueryDescription get_world_renderer_descr("get_world_renderer", {
  {ecs::get_type_description<WorldRenderer>("wr"), false}
});

template<typename Callable>
void get_world_renderer(Callable lambda)
{
  ecs::perform_query<WorldRenderer&>
  (get_world_renderer_descr, lambda);
}


ecs::QueryDescription gather_sprites_descr("gather_sprites", {
  {ecs::get_type_description<Asset<Texture2D>>("mapTexture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("borderTexture"), false},
  {ecs::get_type_description<Shader>("mapShader"), false},
  {ecs::get_type_description<vector<vec3>>("land_colors"), false},
  {ecs::get_type_description<vec4>("color"), true}
});

template<typename Callable>
void gather_sprites(Callable lambda)
{
  ecs::perform_query<Asset<Texture2D>&, Asset<Texture2D>&, Shader&, vector<vec3>&, vec4*>
  (gather_sprites_descr, lambda);
}


void render_scene_func();

ecs::SystemDescription render_scene_descr("render_scene", {
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, render_scene_func, ecs::SystemOrder::RENDER, ecs::SystemTag::Game,
{},
{});

void render_scene_func()
{
  ecs::perform_system(render_scene_descr, render_scene);
}

void update_map_textures_func();

ecs::SystemDescription update_map_textures_descr("update_map_textures", {
  {ecs::get_type_description<bool>("mapWasChanged"), false},
  {ecs::get_type_description<Asset<Texture2D>>("mapTexture"), false},
  {ecs::get_type_description<MapArrays>("map_arrays"), false}
}, update_map_textures_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game,
{},
{});

void update_map_textures_func()
{
  ecs::perform_system(update_map_textures_descr, update_map_textures);
}


