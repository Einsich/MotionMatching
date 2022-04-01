#include <ecs.h> 
#include <render/render.h> 
#include <transform.h>

#include <render/texture/stb_image.h>
Asset<Mesh> create_detailed_plane(uint h, uint w) 
{ 
  vector<uint> indices; 
  vector<vec3> poses, normes; 
  vector<vec2> uves; 
   
  for (uint i = 0; i <= h; i++) 
    for (uint j = 0; j <= w; j++) 
    { 
      float y = i * 1.f / h, x = j * 1.f / w; 
      poses.push_back(vec3(x, 0.f, y)); 
      normes.push_back(vec3(0.f, 1.f, 0.f)); 
      uves.push_back(vec2(x,y)); 
    } 
  for (uint i = 0; i < h; i++) 
    for (uint j = 0; j < w; j++) 
    { 
      uint a = i * (w+1) + j, b = a + 1, c = a + w+1, d = c + 1; 
      indices.insert(indices.end(),{a,b,c, b,d,c}); 
    } 
  return Asset<Mesh>("flag mesh", VertexArrayObject(indices, poses, normes, uves)); 
} 


EVENT(ecs::SystemTag::GameEditor) create_terrain(const ecs::OnEntityCreated&,
  const Asset<Texture2D> &heights_texture,
  const Asset<Texture2D> &provinces_texture,
  const Asset<Texture2D> &normal_texture,
  const Asset<Texture2D> &trees_texture,
  const Asset<Texture2D> &rivers_texture,
  const Asset<Texture2D> &terrain_texture,
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
  mesh = create_detailed_plane(h/10, w/10);
  /* 
  const auto &path = heights_texture.asset_path();
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.string().c_str(), &w, &h, &ch, 0); */

  material->set_texture("heightMap", heights_texture);
  material->set_texture("provincesMap", provinces_texture);
  material->set_texture("normalMap", normal_texture);
  material->set_texture("treesMap", trees_texture);
  material->set_texture("riversMap", rivers_texture);
  material->set_texture("terrainMap", terrain_texture);
}