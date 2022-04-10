#include <ecs.h>
#include <render/render.h>
#include <input.h>
#include <camera.h>
#include <imgui.h>
#include <application/file_dialog.h>
#include "political_map.h"
#include "heightmap.h"

struct MapEditor : ecs::Singleton
{
  bool editProvinces = false;
  bool enableWater = true;
  int currentCountry = -1;
  int currentCountryId = 0xffff;
};

template<typename Callable>
static void toggle_water(Callable);

SYSTEM(ecs::SystemOrder::UI, ecs::SystemTag::Editor) country_builder(
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
      keys.emplace_back(0xffff);
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
      QUERY(ecs::Tag isWater)toggle_water([&](bool &is_enabled)
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
    ImGui::End();
  }
}

EVENT(ecs::SystemTag::Editor) trace_province(
  const MouseButtonDownEvent<MouseButton::LeftButton> &event,
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
  float x = event.x, y = event.y;

  auto [w, h] = get_resolution();
  
  vec2 screenPos((x / w * 2.f - 1.0f)*mainCamera.aspectRatio, 1.0f - y / h * 2.f);
  vec3 p = mainCamera.transform * vec4(screenPos, 1, 1);
  vec3 n = mainCamera.transform * vec4(screenPos, 1, 0);
  n = normalize(n);
  vec3 worldPos;
  if (heightMap.ray_trace(p, n, worldPos))
  {
    int x,y;
    if (heightMap.world_to_pixel(worldPos, x, y))
    {
      uint id = politicalMap.provincesIdx[x+y*heightMap.w];
      politicalMap.provincesInfo[id].x = editor.currentCountryId;
      political_material->set_property("material.provincesInfo[0]", politicalMap.provincesInfo);
    }

  }
}