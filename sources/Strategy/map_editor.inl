#include <ecs/ecs.h>
#include <render/render.h>
#include <input.h>
#include <camera.h>
#include <imgui.h>
#include <application/file_dialog.h>
#include <application/time.h>
#include "political_map.h"
#include "heightmap.h"
#include "map_render_data.h"
#include <ecs/imgui.h>

struct MapEditor : ecs::Singleton
{
  bool editProvinces = false;
  bool enableWater = true;
  int currentCountry = -1;
  int currentCountryId = 0xffff;
};

ECS_REGISTER_SINGLETON(MapEditor)
ECS_REGISTER_SINGLETON(HeightMap)

template<typename Callable>
static void toggle_water(Callable);

EVENT(tags=editor) country_builder(const ImguiRender&, 
  MapEditor &editor,
  const PoliticalMap &politicalMap)
{
  if (ImGui::Begin("MapEditor"))
  {    
    ImGui::Checkbox("Edit provinces", &editor.editProvinces);
    if (editor.editProvinces)
    {
      vector<const char *> names;
      vector<int> keys;
      names.emplace_back("None");
      keys.emplace_back(PoliticalMap::MAX_PROVINCES);
      for (const auto &state : politicalMap.countries)
      {
        names.emplace_back(state.name.c_str());
        keys.emplace_back(state.id);
      }

      if (editor.currentCountry >= 0)
        ImGui::Text("Current state; %s", names[editor.currentCountry]);
      if (ImGui::ListBox("Select state", &editor.currentCountry, names.data(), names.size(), 10))
      {
        editor.currentCountryId = keys[editor.currentCountry];
      }
    }
    if (ImGui::Checkbox("Toggle water", &editor.enableWater))
    {
      QUERY(require=ecs::Tag isWater)toggle_water([&](bool &is_enabled)
      {
        is_enabled = editor.enableWater;
      });
    }
    string provincesFile;
    if (ImGui::Button("Save provinces") && get_save_file(provincesFile, ".blk"))
    {
      DataBlock provicesData;
      vector<DataBlock *> states(politicalMap.countries.size());
      for (int i = 0, n = politicalMap.countries.size(); i < n; i++)
        states[i] = provicesData.addBlock(politicalMap.countries[i].name.c_str());
      for (int i = 0, n = politicalMap.provincesInfo.size(); i < n; i++)
      {
        uint state = politicalMap.provincesInfo[i].x;
        if (state < politicalMap.countries.size())
        {
          states[state]->add("province", i);
        }
      }
      provicesData.save(provincesFile);
    }
  }
    ImGui::End();
}

bool get_map_hit(
  float screen_x,
  float screen_y,
  const MainCamera &camera,
  const HeightMap &height_map,
  const PoliticalMap &political_map,
  vec3 &world_pos,
  uint &prov_id)
{
  auto [w, h] = get_resolution();
  vec2 screenPos((screen_x / w * 2.f - 1.0f)*camera.aspectRatio, screen_y / h * 2.f - 1.0f);
  vec3 p = camera.transform * vec4(screenPos, 1, 1);
  vec3 n = camera.transform * vec4(screenPos, 1, 0);
  n = normalize(n);
  if (height_map.ray_trace(p, n, world_pos))
  {
    int x,y;
    if (height_map.world_to_pixel(world_pos, x, y))
    {
      prov_id = political_map.provincesIdx[x+y*height_map.w];
      return true;
    }
  }
  return false;
}

EVENT(tags=editor) trace_province(
  const MouseClickEvent &event,
  Asset<Material> &political_material,
  const MapEditor &editor,
  const MainCamera &mainCamera,
  const HeightMap &heightMap,
  PoliticalMap &politicalMap)
{
  if (event.action != MouseAction::Down ||
      !editor.editProvinces ||
      editor.currentCountry < 0 ||
      ImGui::IsAnyWindowHovered())
    return;
  int editedProvId = event.buttonType == MouseButton::RightButton ? PoliticalMap::MAX_PROVINCES : editor.currentCountryId;

  vec3 worldPos;
  uint provId;
  if (get_map_hit(event.x, event.y, mainCamera, heightMap, politicalMap, worldPos, provId))
  {
    politicalMap.provincesInfo[provId].x = editedProvId;
    political_material->set_property("provincesInfo[0]", politicalMap.provincesInfo);
  }
}

static void selection_province(const PoliticalMap &political_map, MapRenderData &render_data, uint province_id, bool selection)
{
  if (province_id >= political_map.provinces.size())
    return;
  for (int borderId : political_map.provinces[province_id].borderIndexes)
    render_data.borders[borderId].color1.a = selection ? Time::time() : 0;
}

EVENT() selecte(
  const MouseClickEvent &event,
  const MainCamera &mainCamera,
  const HeightMap &heightMap,
  const PoliticalMap &politicalMap,
  MapRenderData &renderData)
{
  if (event.action != MouseAction::Down || event.buttonType != MouseButton::LeftButton ||
      ImGui::IsAnyWindowHovered())
    return;

  vec3 worldPos;
  uint provId;
  static uint prevSelectedProv = -1;
  if (get_map_hit(event.x, event.y, mainCamera, heightMap, politicalMap, worldPos, provId))
  {
    selection_province(politicalMap, renderData, prevSelectedProv, false);
    if (prevSelectedProv != provId)
    {
      selection_province(politicalMap, renderData, provId, true);
      prevSelectedProv = provId;
    }
    else
    {
      prevSelectedProv = -1;
    }
  }
}