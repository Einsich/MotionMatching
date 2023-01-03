#include <ecs/ecs.h> 
#include <render/render.h> 
#include <render/texture/stb_image.h>
#include <render/texture/stb_write.h>
#include <input.h>
#include "political_map.h"
#include "map_render_data.h"

ECS_REGISTER_SINGLETON(PoliticalMap)
ECS_REGISTER_SINGLETON(MapRenderData)


Asset<Mesh> build_borders(PoliticalMap &political_map, float pixel_scale);

template<typename Callable>
static void toggle_map_mode(Callable);

EVENT() change_terrain_mode(
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
  QUERY(require=ecs::Tag isTree)toggle_map_mode([&](bool &is_enabled)
  {
    is_enabled = !politicalMode;
  });
}

EVENT() create_provinces(const ecs::OnSceneCreated&,
  Asset<Material> &political_material,
  Asset<Texture2D> &provinces_texture,
  const ecs::string &provinces_texture_name,
  const ecs::string &load_provinces_info,
  float pixel_scale,
  PoliticalMap &politicalMap,
  MapRenderData &data)
{
  DataBlock countries(root_path("resources/Strategy/Content/countries.blk"));
  DataBlock provincesOwn(root_path(load_provinces_info.c_str()));
  auto &states = politicalMap.countries;
  eastl::vector<vec3> countriesColors;
  for (uint i = 0; i < countries.blockCount(); i++)
  {
    const DataBlock *state = countries.getBlock(i);
    vec3 color = state->get("color", ivec3(0,0,0));
    color /= 255.f;
    //color = glm::pow(color / 255.f, vec3(2.5f));
    countriesColors.emplace_back(color);
    states.push_back({color, (int)i, state->name()});
  }

  const auto &path = root_path(provinces_texture_name.c_str());
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto img = stbi_load(path.c_str(), &w, &h, &ch, 4);
  politicalMap.w = w;
  politicalMap.h = h;
  auto &provincesMap = politicalMap.provincesIdx;  
  provincesMap.resize(w*h);
  memcpy(provincesMap.data(), img, w*h*4);
  stbi_image_free(img);

  uint provincesCount = PoliticalMap::MAX_PROVINCES;

  auto &provincesInfo = politicalMap.provincesInfo;
  politicalMap.provinces.resize(provincesCount);
  provincesInfo.resize(provincesCount, uvec2(PoliticalMap::MAX_PROVINCES, PoliticalMap::MAX_PROVINCES));
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
  //stbi_flip_vertically_on_write(1);
  //stbi_write_tga(root_path("resources/Strategy/Content/provinces.tga").c_str(), w, h, 4, provincesMap.data());

  political_material.try_load();
  political_material->set_property("stateColor[0]", countriesColors);
  political_material->set_property("provincesInfo[0]", provincesInfo);
  political_material->set_texture("provincesMap", provinces_texture);
  

  {
    ecs::create_entity("borders", {{"mesh", build_borders(politicalMap, pixel_scale)}});
    for (auto [key, value] : politicalMap.borderIndexes)
    {
      uint prov0 = key & PoliticalMap::PROVINCES_MASK;
      uint prov1 = (key >> 16u) & PoliticalMap::PROVINCES_MASK;

      uint owner0 = politicalMap.provincesInfo[prov0].x;
      uint owner1 = politicalMap.provincesInfo[prov1].x;
      if (owner0 == owner1 || owner0 == PoliticalMap::MAX_PROVINCES || owner1 == PoliticalMap::MAX_PROVINCES)
        data.borders[value.first] = {vec4(0,0,0, 0), vec4(0,0,0, 0)};
      else
      {
        if (!value.second)
          std::swap(owner0, owner1);
        data.borders[value.first] = {
          vec4(politicalMap.countries[owner0].color, 1),
          vec4(politicalMap.countries[owner1].color, 0)
        };
      }
    }
  }
}