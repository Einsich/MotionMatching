#include <ecs.h> 
#include <render/shader/shader.h>
#include <render/texture/texture2d.h>
#include <transform2d.h>
#include "map_arrays.h"
#include "world_render.h"

ECS_DECLARE_TYPE(WorldRenderer);

template<typename Callable>
void get_world_renderer(Callable);

vec2 screen_to_world(int x, int y)
{
  vec2 result;
  QUERY()get_world_renderer([&](const WorldRenderer &wr) { result = wr.screen_to_world(x,y);});
  return result;
}

template<typename Callable>
void gather_sprites(Callable);

SYSTEM(ecs::SystemOrder::RENDER) render_scene(
  const mat3 &cameraProjection,
  const Transform2D &transform,
  WorldRenderer &wr)
{
  static vector<uint> indices = {0,1,2,0,2,3};
  static vector<vec2> corners =  {vec2(-1, -1), vec2(1,-1), vec2(1,1),vec2(-1,1)};
  static VertexArrayObject squadVao(indices, corners);
  wr.screenToWorld = transform.get_matrix();
  mat3 viewProjection = cameraProjection * glm::inverse(wr.screenToWorld);
  QUERY() gather_sprites([&](
    const Asset<Texture2D> &mapTexture,
    const Asset<Texture2D> &borderTexture,
    Shader &mapShader,
    vector<vec3> &land_colors,
    const vec4 *color)
  {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    mapShader.use();

    mapTexture->bind(mapShader, "sprite", 0);
    borderTexture->bind(mapShader, "border", 1);
    mapShader.set_mat3x3("trasformViewProjection", viewProjection);
    mapShader.set_vec4("color", color ? *color : vec4(1.f));
    mapShader.set_vec3("colors", land_colors);
    squadVao.render();
    glEnable(GL_DEPTH_TEST);
  });

}

SYSTEM()update_map_textures(
  bool &mapWasChanged,
  Asset<Texture2D> &mapTexture,
  const MapArrays &map_arrays
)
{
  if (mapWasChanged)
  {
    auto [width, height] = map_arrays.color_indices.dimension();
    mapTexture->update_sub_region(0, 0, 0, width, height, map_arrays.color_indices.data());
    mapWasChanged = false;
  }
}