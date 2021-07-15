#include "ecs/ecs.h"
#include "Engine/imgui/imgui.h"
#include "Engine/Resources/resources.h"
#include "ecs/singleton.h"
struct SelectedAsset : ecs::Singleton
{
  Asset<AssetStub> *asset = nullptr;
};
SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::GameEditor) resources_menu(SelectedAsset &selectedAsset)
{

  if (ImGui::BeginMenu("Resources"))
  {
    auto & assets = Resources::instance().assets;
    for (auto &p : assets)
    {
      if (ImGui::TreeNode(p.first.data()))
      {
        for (auto &asset : p.second.resources)
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
              p.second.loadAsset(asset.second, false);
            }
          }
          if (asset.second.loaded() && selectedAsset.asset == &asset.second)
            p.second.editAsset(asset.second);
        }
        ImGui::TreePop();
      }
    }
    ImGui::EndMenu();
  }
}

