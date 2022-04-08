#include <ecs.h> 
#include <render/render.h> 
#include <render/texture/stb_image.h>
#include <input.h>
#include "political_map.h"

template<typename Callable>
static void toggle_map_mode(Callable);

EVENT(ecs::SystemTag::GameEditor) change_terrain_mode(
  const KeyDownEvent<SDLK_m> &,
  Asset<Material> &material,
  const Asset<Material> &political_material,
  const Asset<Material> &physycal_material)
{
  static int k = 0;
  k ^= 1;
  bool politicalMode = k == 1;
  if (politicalMode)
    material = political_material;
  else
    material = physycal_material;
  QUERY(ecs::Tag isTree)toggle_map_mode([&](bool &is_enabled)
  {
    is_enabled = !politicalMode;
  });
}

EVENT(ecs::SystemTag::GameEditor) create_provinces(const ecs::OnSceneCreated&,
  Asset<Material> &political_material,
  Asset<Texture2D> &provinces_texture,
  const string &provinces_texture_name,
  PoliticalMap &politicalMap)
{
  
  const auto &path = root_path(provinces_texture_name);
  DataBlock countries(root_path("resources/Strategy/Content/countries.blk"));

  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.c_str(), &w, &h, &ch, 0);
  auto imPtr = image;
  map<uint, int> colorMap;
  auto &states = politicalMap.countries;
  vector<vec3> countriesColors;
  for (uint i = 0; i < countries.blockCount(); i++)
  {
    const DataBlock *state = countries.getBlock(i);
    vec3 color = state->get("color", ivec3(0,0,0));
    color = glm::pow(color / 255.f, vec3(2.5f));
    countriesColors.emplace_back(color);
    states.push_back({color, (int)i, state->name()});
  }
  auto &provincesMap = politicalMap.provincesIdx;
  provincesMap.resize(h*w);
  int cnt = 0;
  for (int i = 0, n = h * w; i < n; i++, imPtr += ch)
  {
    uint c = (imPtr[0] << 16u) + (imPtr[1] << 8u) + imPtr[2];
    auto it = colorMap.find(c);
    if (it == colorMap.end())
    {
      provincesMap[i] = cnt;
      colorMap[c] = cnt++;
    }
    else
    {
      provincesMap[i] = it->second;
    }    
  }
  auto &provincesInfo = politicalMap.provincesInfo;
  provincesInfo.resize(colorMap.size(), uvec2(0xffff, 0xffff));

  provinces_texture = Asset<Texture2D>(
    Texture2D(w, h, TextureColorFormat::RI, TextureFormat::UnsignedInt, TexturePixelFormat::Pixel, TextureWrappFormat::ClampToEdge));
  provinces_texture->update_sub_region(0,0,0,w,h,provincesMap.data());
  debug_log("%d", colorMap.size());
  political_material->set_property("material.stateColor[0]", countriesColors);
  political_material->set_property("material.provincesInfo[0]", provincesInfo);
  political_material->set_texture("provincesMap", provinces_texture);
}