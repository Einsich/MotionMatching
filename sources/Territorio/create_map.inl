#include <ecs.h> 
#include <type_registration.h>
#include <transform2d.h>
#include <render/texture/texture2d.h>
#include "map_arrays.h"

ECS_REGISTER_TYPE(MapArrays, MapArrays, false, false)


static vector<ivec2> get_player_start_points(int h, int w, int n)
{ 
  vector<ivec2> points(n);
  for (int i = 0; i < n; i++)
  {
again:
    ivec2 p(rand_int(w), rand_int(h));
    for (int j = 0; j < i; j++)
      if (length(vec2(p - points[j])) < 10.f)
        goto again;
    points[i] = p;
  }
  return points;
}

template<typename Callable>
static void spawn_player_query(Callable);

ecs::EntityId spawn_player(const ecs::Template *player_template, ivec2 start_point)
{
  ecs::EntityId result;
  QUERY()spawn_player_query([&](
    MapArrays &map_arrays,
    vector<vec3> &land_colors,
    bool &mapWasChanged,
    bool &needUpdateBorder)
  {
    uint i = map_arrays.borders.size();
    auto &borders = map_arrays.borders.emplace_back(start_point);
    ivec2 p = start_point;
    map_arrays.color_indices[p.y][p.x] = i;
    for (ivec2 d : {ivec2(1, 0), ivec2(-1, 0), ivec2(0, -1), ivec2(0, 1)})
      if (map_arrays.color_indices.test(p.y + d.y, p.x + d.x))  
        borders.insert(p+d);
    land_colors[i] = rand_vec3(0, 1);
    ecs::ComponentInitializerList list;
    list.set("spawnPoint", p);
    list.set("landIndex", (uint)i);
    result = ecs::create_entity(player_template, std::move(list));
    needUpdateBorder = mapWasChanged = true;
  });
  return result;
}

EVENT() create_map(
  const ecs::OnEntityCreated &,
  int width,
  int height,
  Asset<Texture2D> &mapTexture,
  Asset<Texture2D> &borderTexture,
  Shader &mapShader,
  MapArrays &map_arrays,
  vector<vec3> &land_colors,
  int botsCount)
{
  array2d<uint> &color_indices = map_arrays.color_indices;
  mapTexture = Asset<Texture2D>(
    Texture2D(width, height, TextureColorFormat::RI, TextureFormat::UnsignedInt, TexturePixelFormat::Pixel));
  borderTexture = Asset<Texture2D>(
    Texture2D(width, height, TextureColorFormat::RI, TextureFormat::UnsignedInt, TexturePixelFormat::Pixel));
  mapShader = get_shader("map_shader");
  constexpr int MAX_COLORS = 1024;
  uint grayLandIdx =  MAX_COLORS-1;
  color_indices = array2d<uint>(height, width);
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      color_indices[i][j] = grayLandIdx;
  land_colors.resize(MAX_COLORS, vec3(0.7f));
  ecs::EntityId grayLand;
  {
    const ecs::Template *landTemplate = ecs::get_template("land");
    ecs::ComponentInitializerList list;
    list.set("landIndex", grayLandIdx);
    list.set("landCount", height*width - botsCount);
    grayLand = ecs::create_entity(landTemplate, std::move(list));
  }

  vector<ivec2> startPoints = get_player_start_points(width, height, botsCount);
  const ecs::Template *botTemplate = ecs::get_template("bot");
  for (int i = 0; i < botsCount; i++)
  {
    spawn_player(botTemplate, startPoints[i]);
  }

}
