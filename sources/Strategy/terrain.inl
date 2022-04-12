#include <ecs.h> 
#include <render/render.h> 
#include <transform.h>
#include <render/texture/stb_image.h>
#include <camera.h>
#include "heightmap.h"
#include <render/global_uniform.h>
#include "map_render_data.h"

EVENT(ecs::SystemTag::GameEditor) add_map_uniform(const ecs::OnSceneCreated &)
{
  add_uniform_buffer<MapRenderData>("mapData", 1);
}
SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor)
set_map_render_data(const MapRenderData &data)
{
  get_buffer("mapData").update_buffer_and_flush<MapRenderData>(data); 
}
static Asset<Mesh> create_detailed_plane(uint h, uint w, int lod) 
{ 
  uint tris = h * w * 2 * 3;
  uint vert = (h+1)*(w+1);
  vector<uint> indices(tris); 
  vector<vec3> poses(vert), normes(vert); 
  vector<vec2> uves(vert); 

  int k = 0;
  float dx = 1.f / w, dy = 1.f / h;
  for (uint i = 0; i <= h; i++) 
    for (uint j = 0; j <= w; j++) 
    { 
      float y = i * dy, x = j * dx; 
      poses[k] = vec3(x, 0.f, y); 
      normes[k] = vec3(0.f, 1.f, 0.f); 
      uves[k] = vec2(x,y);
      k++; 
    } 
  k = 0;
  for (uint i = 0; i < h; i++) 
    for (uint j = 0; j < w; j++) 
    { 
      uint a = i * (w+1) + j, b = a + 1, c = a + w+1, d = c + 1; 
      indices[k+0]=a;
      indices[k+1]=b;
      indices[k+2]=c;      
      indices[k+3]=b;
      indices[k+4]=d;
      indices[k+5]=c;
      k += 6;
    }
  string name = "terrain_lod[" + to_string(lod) + "]";
  return Asset<Mesh>(name, indices, poses, normes, uves); 
} 

static vector<uint> terrain_types(
  const string &terrain_texture,
  const vector<ivec3> &terrain_type_color,
  const vector<int> &terrain_type_index)
{
  const auto &path = root_path(terrain_texture);
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.c_str(), &w, &h, &ch, 0);
  auto imPtr = image;
  vector<uint> terrainTypes(h*w, terrain_type_color.size());
  map<uint, int> colorMap;
  for (uint j = 0; j < terrain_type_color.size(); j++)
  {
    ivec3 c = terrain_type_color[j];
    uint key = (c.r<<16u) + (c.g<<8u) + c.b;
    colorMap.insert({key, terrain_type_index[j]});
  }
  for (uint i = 0, n = w * h; i < n; i++, imPtr += 3)
  {
    uint key = (imPtr[0]<<16u) + (imPtr[1]<<8u) + imPtr[2];
    auto it = colorMap.find(key);
    if (it != colorMap.end())
    {
      terrainTypes[i] = it->second;
    }
  }
  stbi_image_free(image);
  return terrainTypes;
}

//ugly hardcoded tree spawn
static void spawn_tress(
  const string &tree_map,
  vec2 map_scale,
  float tree_scale,
  const HeightMap &height_map
)
{
  const auto &path = root_path(tree_map);
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.c_str(), &w, &h, &ch, 0);
  auto imPtr = image;
  const char *material_names[3] = {"tree_leafs", "tree_pine", "tree_palms"};
  vec2 texelSize = vec2(1.f / map_scale.x, 1.f / map_scale.y);
  for (int i = 0; i < 3; i++)
  {
    Asset<Material> leaf = get_resource<Material>(material_names[i]);
    if (leaf)
    {
      leaf->set_property("material.mapTexelSize", texelSize);
      leaf->before_save();
    }
  }
  const ecs::Template *leafs[3] = {
    ecs::get_template("leafs_1"),
    ecs::get_template("leafs_2"),
    ecs::get_template("leafs_3")
  };
  const ecs::Template *pines[3] = {
    ecs::get_template("pine_1"),
    ecs::get_template("pine_2"),
    ecs::get_template("pine_3")
  };
  const ecs::Template *palms[1] = {
    ecs::get_template("palms_1")
  };
  vec2 d = vec2(1.f/w, 1.f/h);
  for (uint i = 0, n = w * h; i < n; i++, imPtr += 3)
  {
    int r = imPtr[0] >> 6u;
    int g = imPtr[1] >> 6u;
    int b = imPtr[2] >> 6u;
    uint key = (r<<16u) + (g<<8u) + b;

    if (key!=0)
    {
      const ecs::Template *treeTempalte = nullptr;
      if (b >= g && r == 0)
        treeTempalte = pines[(b+g)*3 / 8];
      else if (imPtr[0] >= imPtr[1] )
        treeTempalte = palms[0];
      else if (g > 0)
        treeTempalte = leafs[(b+g)*3 / 8 ];
      

      if (!treeTempalte)
        continue;
      vec2 offsets[4] = {vec2(0, 0),vec2(0, 1.f),vec2(1.f, 0), vec2(1.f, 1.f)};
      for (int k = 0; k < 4; k ++)
      {

        vec2 p = (vec2(i % w + 0.5f, i / w) + offsets[k]*0.5f + rand_vec2() * 0.25f) * map_scale * d ;
        float height;
        bool onLand = 
            height_map.is_land(p + vec2(1, 1) * tree_scale, height) &&
            height_map.is_land(p + vec2(1, -1) * tree_scale, height) &&
            height_map.is_land(p + vec2(-1, 1) * tree_scale, height) &&
            height_map.is_land(p + vec2(-1, -1) * tree_scale, height);

        if (onLand && height_map.is_land(p, height))
        {
          ecs::ComponentInitializerList list;
          list.set("transform", Transform(vec3(p.x, height, p.y), vec3(rand_float() * PI,0,0), vec3(tree_scale)));
          list.set("is_enabled", false);
          ecs::create_entity(treeTempalte, std::move(list));
        }
      }
    }
  }
  stbi_image_free(image);

}
template<typename Callable>
static void query_water(Callable);

EVENT(ecs::SystemTag::GameEditor) create_terrain(const ecs::OnSceneCreated&,
  const Asset<Texture2D> &heights_texture,
  const Asset<Texture2D> &normal_texture,
  const Asset<Texture2DArray> &terrain_diffuse_array,
  const Asset<Texture2DArray> &terrain_normal_array,
  const Asset<Texture2DArray> &terrain_colormap_array,
  Asset<Texture2D> &terrain_types_texture,
  Asset<Material> &material,
  Asset<Material> &political_material,
  Asset<Material> &physycal_material,
  Transform &transform,
  vector<float> &lods_distances,
  vector<Asset<Mesh>> &lods_meshes,
  int terrain_lods_count,
  float first_lod_distance,
  const string &terrain_texture,
  const string &tree_map,
  float tree_scale,
  const vector<ivec3> &terrain_type_color,
  const vector<int> &terrain_type_index,
  float pixel_scale,
  int water_level,
  vec2 &map_size,
  HeightMap &heigth_map,
  MapRenderData &data
)
{
  if (!heights_texture)
  {
    debug_error("terrain has invalid heights_texture");
    return;
  }

  int w = heights_texture->width(), h = heights_texture->height();
  float mapWidth = w * pixel_scale;
  float mapHeight = h * pixel_scale;
  map_size = vec2(mapWidth, mapHeight);
  data.mapSize = vec4(map_size, 1.f / map_size);
  transform.set_scale(vec3(mapWidth, 1, mapHeight));
  
  string path = heights_texture.asset_path().string();
  path.resize(path.size() - sizeof("meta"));
  heigth_map.load_heightmap(path, 1.f, water_level);
  heigth_map.worldOffset = vec2(0, 0);
  heigth_map.worldScale = vec2(1.f) / pixel_scale;
  lods_distances.resize(terrain_lods_count);
  lods_meshes.resize(terrain_lods_count);
  for (int i = 0; i < terrain_lods_count; i++)
  {
    int power = 1 << i;
    lods_distances[i] = first_lod_distance * power;
    uint p = i + 1;
    lods_meshes[i] = create_detailed_plane(h >> p, w >> p, i);
  }

  terrain_types_texture = Asset<Texture2D>(
    Texture2D(w, h, TextureColorFormat::RI, TextureFormat::UnsignedInt, TexturePixelFormat::Pixel, TextureWrappFormat::ClampToEdge));
  if (terrain_type_color.size() == terrain_type_index.size())
  {
    vector<uint> terrainTypes = terrain_types(terrain_texture, terrain_type_color, terrain_type_index);
    terrain_types_texture->update_sub_region(0, 0, 0, w, h, terrainTypes.data());
  }
  else
    debug_error("terrain_type_color and terrain_type_index should have same size");

  spawn_tress(tree_map, vec2(mapWidth, mapHeight), tree_scale, heigth_map);
  auto materialSetter = [&](Material &mat)
  {
    mat.set_texture("heightMap", heights_texture);
    mat.set_texture("normalMap", normal_texture);
    mat.set_texture("terrainMap", terrain_types_texture);
    mat.set_texture("terrainDiffuseArray", terrain_diffuse_array);
    mat.set_texture("terrainNormalArray", terrain_normal_array);
    mat.set_texture("terrainColormapArray", terrain_colormap_array);
    mat.set_property("material.mapSize", ivec2(w, h));
    mat.set_property("material.texelSize", vec2(1.f / w, 1.f / h));
    mat.before_save();
  };
  materialSetter(*political_material);
  materialSetter(*physycal_material);
  material = political_material;

  QUERY(ecs::Tag isWater)query_water([&](
    const Asset<Texture2D> &water_noise_texture,
    const Asset<Texture2D> &water_color_texture,
    const Asset<Texture2D> &water_foam_texture,
    const Asset<CubeMap> &sky_reflection,
    Asset<Material> &material,
    Transform &transform)
  {
    float waterLevel = (water_level+0.5f)/255.f;
    float wScale = mapWidth * 0.5f;
    float hScale = mapHeight * 0.5f;
    transform.set_position(vec3(wScale, waterLevel, hScale));
    transform.set_scale(vec3(wScale, 1, hScale));
    material->set_texture("heightMap", heights_texture);
    material->set_texture("waterNoise", water_noise_texture);
    material->set_texture("waterColor", water_color_texture);
    material->set_texture("waterFoam", water_foam_texture);
    material->set_texture("skyReflection", sky_reflection);
    material->set_property("material.waterLevel", waterLevel);
    material->before_save();
  });
}
