#include <ecs.h> 
#include <render/render.h> 
#include <transform.h>
#include <render/texture/stb_image.h>
#include <camera.h>

Asset<Mesh> create_detailed_plane(uint h, uint w, int lod) 
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
  return Asset<Mesh>(name, VertexArrayObject(indices, poses, normes, uves)); 
} 

vector<uint> terrain_types(
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

EVENT(ecs::SystemTag::GameEditor) create_terrain(const ecs::OnEntityCreated&,
  const Asset<Texture2D> &heights_texture,
  const Asset<Texture2D> &provinces_texture,
  const Asset<Texture2D> &normal_texture,
  const Asset<Texture2DArray> &terrain_diffuse_array,
  const Asset<Texture2DArray> &terrain_normal_array,
  const Asset<Texture2DArray> &terrain_colormap_array,
  Asset<Texture2D> &terrain_types_texture,
  Asset<Material> &material,
  Transform &transform,
  vector<float> &lods_distances,
  vector<Asset<Mesh>> &lods_meshes,
  int terrain_lods_count,
  float first_lod_distance,
  const string &terrain_texture,
  const vector<ivec3> &terrain_type_color,
  const vector<int> &terrain_type_index,
  float pixel_scale
)
{
  if (!heights_texture)
  {
    debug_error("terrain has invalid heights_texture");
    return;
  }

  int w = heights_texture->width(), h = heights_texture->height();
  transform.set_scale(vec3(w * pixel_scale, 1, h * pixel_scale));
  
  lods_distances.resize(terrain_lods_count);
  lods_meshes.resize(terrain_lods_count);
  for (int i = 0; i < terrain_lods_count; i++)
  {
    int power = 1 << i;
    lods_distances[i] = first_lod_distance * power;
    lods_meshes[i] = create_detailed_plane(h >> i, w >> i, i);
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


  material->set_texture("heightMap", heights_texture);
  material->set_texture("provincesMap", provinces_texture);
  material->set_texture("normalMap", normal_texture);
  material->set_texture("terrainMap", terrain_types_texture);
  material->set_texture("terrainDiffuseArray", terrain_diffuse_array);
  material->set_texture("terrainNormalArray", terrain_normal_array);
  material->set_texture("terrainColormapArray", terrain_colormap_array);
  material->set_property("material.mapSize", ivec2(w, h));
  material->set_property("material.texelSize", vec2(1.f / w, 1.f / h));
  material->before_save();
}
