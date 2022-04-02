#include <ecs.h> 
#include <render/render.h> 
#include <transform.h>
#include <render/texture/stb_image.h>

Asset<Mesh> create_detailed_plane(uint h, uint w) 
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
  return Asset<Mesh>("flag mesh", VertexArrayObject(indices, poses, normes, uves)); 
} 


EVENT(ecs::SystemTag::GameEditor) create_terrain(const ecs::OnEntityCreated&,
  const Asset<Texture2D> &heights_texture,
  const Asset<Texture2D> &provinces_texture,
  const Asset<Texture2D> &normal_texture,
  const Asset<Texture2D> &terrain_texture,
  const Asset<Texture2DArray> &terrain_diffuse_array,
  const Asset<Texture2DArray> &terrain_normal_array,
  Asset<Material> &material,
  Transform &transform,
  Asset<Mesh> &mesh,
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
  mesh = create_detailed_plane(h, w);
  /* 
  const auto &path = heights_texture.asset_path();
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.string().c_str(), &w, &h, &ch, 0); */

  material->set_texture("heightMap", heights_texture);
  material->set_texture("provincesMap", provinces_texture);
  material->set_texture("normalMap", normal_texture);
  material->set_texture("terrainMap", terrain_texture);
  material->set_texture("terrainDiffuseArray", terrain_diffuse_array);
  material->set_texture("terrainNormalArray", terrain_normal_array);
}