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
  const string &load_provinces_info,
  PoliticalMap &politicalMap)
{
  
  const auto &path = root_path(provinces_texture_name);
  

  int w, h, ch;
  if (!stbi_info(path.c_str(), &w, &h, &ch))
  {
    debug_error("hasn't texture %s", provinces_texture_name.c_str());
    return;
  }
  DataBlock countries(root_path("resources/Strategy/Content/countries.blk"));
  DataBlock provincesOwn(root_path(load_provinces_info));
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

  load_object_path(politicalMap.provincesIdx, root_path("resources/Strategy/Content/provinces.bin"));

  auto &provincesMap = politicalMap.provincesIdx;  
  uint provincesCount = politicalMap.provincesIdx.back();
  provincesMap.pop_back();

  auto &provincesInfo = politicalMap.provincesInfo;
  provincesInfo.resize(provincesCount, uvec2(0xffff, 0xffff));
  for (int i = 0, n = provincesOwn.blockCount(); i < n; i++)
  {
    const DataBlock *state = provincesOwn.getBlock(i);
    
    auto it = std::find_if(states.begin(), states.end(), [state](const auto &s){return s.name == state->name();});
    if (it == states.end())
    {
      debug_error("doesn't exist %s state", state->name().c_str());
      continue;
    }
    int stateIdx = it->id;
    for (int j = 0, n = state->propertiesCount(); j < n; j++)
    {
      const auto &prop = state->getProperty(j);
      if (prop.name == "province")
      {
        int provinceId = state->get<int>(prop);
        provincesInfo[provinceId].x = stateIdx;
      }
    }
  }
  provinces_texture = Asset<Texture2D>(
    Texture2D(w, h, TextureColorFormat::RI, TextureFormat::UnsignedInt, TexturePixelFormat::Pixel, TextureWrappFormat::ClampToEdge));
  provinces_texture->update_sub_region(0,0,0,w,h,provincesMap.data());
  //provincesMap.push_back(provincesCount);
  //save_object_path(provincesMap, root_path("resources/Strategy/Content/provinces.bin"));
  //provincesMap.pop_back();
  debug_log("%d", provincesCount);
  
  political_material->set_property("material.stateColor[0]", countriesColors);
  political_material->set_property("material.provincesInfo[0]", provincesInfo);
  political_material->set_texture("provincesMap", provinces_texture);
}