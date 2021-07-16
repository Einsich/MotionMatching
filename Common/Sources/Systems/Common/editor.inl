#include "ecs/ecs.h"
#include "Engine/imgui/imgui.h"
#include "Engine/Resources/resources.h"
#include "ecs/singleton.h"

struct SelectedAsset : ecs::Singleton
{
  Asset<AssetStub> *asset = nullptr;
  string_view name;
  ResourceMap *resourceType = nullptr;
};
SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) resources_menu(SelectedAsset &selectedAsset)
{
  if (ImGui::BeginMenu("Resources"))
  {
    auto & assets = Resources::instance().assets;
    for (auto &p : assets)
    {
      if (ImGui::Button(p.first.data()))
      {
        selectedAsset.resourceType = &p.second;
        selectedAsset.name = p.first;
      }
    }
    ImGui::EndMenu();
  }
}

SYSTEM(ecs::SystemOrder::UI, ecs::SystemTag::Editor) asset_viewer(SelectedAsset &selectedAsset)
{
  if (selectedAsset.resourceType)
  {
    if (ImGui::Begin("asset viewer"))
    {
      static bool adding = false;
      ImGui::Text("%s", selectedAsset.name.data());
      if (!selectedAsset.resourceType->metaDataAsset)
      {
        if (adding)
        {
          constexpr int BUFN = 255;
          char buf[BUFN];
          static string bufString = "";
          snprintf(buf, BUFN, "%s", bufString.c_str());
          if (ImGui::InputText("", buf, BUFN))
          {
            bufString.assign(buf);
          }
          if (bufString == "")
            ImGui::TextColored(ImVec4(1,0,0,1), "Enter name");
          else
          {
            auto it = selectedAsset.resourceType->resources.find(bufString);
            if (it != selectedAsset.resourceType->resources.end())
            {
              ImGui::TextColored(ImVec4(1,0,0,1), "There is asset with this name");
            }
            else
            {
              ImGui::SameLine();
              if (ImGui::Button("Create"))
              {
                string folder = project_resources_path(selectedAsset.resourceType->name);
                if (!filesystem::exists(folder))
                  filesystem::create_directory(folder);
                selectedAsset.resourceType->createAsset(folder + "\\" + bufString + "." + selectedAsset.resourceType->name);
                adding = false;
                bufString = "";
              }
            }
          }
        }
        else
        {
          ImGui::SameLine();
          if (ImGui::Button("Add asset"))
          {
            adding = true;
          }
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Close"))
      {
        selectedAsset.resourceType = nullptr;
        selectedAsset.asset = nullptr;
        goto end;
      }
      if (!adding)
      for (auto &asset : selectedAsset.resourceType->resources)
      {
        if (ImGui::Button(asset.first.c_str()))
        {
          if (selectedAsset.asset == &asset.second)
          {
            selectedAsset.asset = nullptr;
          }
          else
          {
            selectedAsset.asset = &asset.second;
            selectedAsset.resourceType->loadAsset(asset.second, false);
          }
        }
        if (asset.second.loaded() && selectedAsset.asset == &asset.second)
        {
          if (selectedAsset.resourceType->editAsset(asset.second))
            selectedAsset.resourceType->reloadAsset(asset.second);
        }
      }
    }
end:
    ImGui::End();
  }
}
