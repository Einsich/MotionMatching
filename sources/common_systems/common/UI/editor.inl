#include <ecs/ecs.h>
#include <imgui.h>
#include <application/file_dialog.h>
#include <resources/resources.h>

#include "editor_window.h"
#include "editor.h"
#include <render/texture/texture2d.h>
#include <eastl/vector.h>
#include <memory/tmp_allocator.h>



EditorWidgets::EditorWidgets():
back("back_button.png", vec2(25, 25))
{
  
}
struct SelectedAsset : ecs::Singleton
{
  Asset<AssetStub> *asset = nullptr;
  string_view name;
  ResourceMap *resourceType = nullptr;
  bool selectNewResourceType = false;
};
ECS_REGISTER_SINGLETON(SelectedAsset)
ECS_REGISTER_SINGLETON(EditorWidgets)

SYSTEM(tags=editor; stage=imgui_menu) resources_menu(SelectedAsset &selectedAsset)
{
  if (ImGui::BeginMenu("Resources"))
  {
    auto & assets = Resources::instance().assets;
    for (auto &p : assets)
    {
      if (ImGui::Button(p.first.data()))
      {
        selectedAsset.selectNewResourceType = selectedAsset.resourceType !=  &p.second;
        selectedAsset.resourceType = &p.second;
        selectedAsset.name = p.first;
        if (selectedAsset.selectNewResourceType)
          selectedAsset.asset = nullptr;
      }
    }
    ImGui::EndMenu();
  }
}

SYSTEM(tags=editor; stage=imgui_render) asset_viewer(SelectedAsset &selectedAsset, const EditorUI &ui)//, const EditorWidgets &widgets
{
  constexpr int BUFN = 255;
  char buf[BUFN];
  bool selectNewResourceType = selectedAsset.selectNewResourceType;
  selectedAsset.selectNewResourceType = false;
  static float scroll = 0;
  if (selectedAsset.resourceType)
  {
    if (ImGui::Begin("asset viewer", nullptr, ui.windowFlags))
    {
      ImGui::PushItemWidth(170);
      static bool adding = false;
      adding &= !selectNewResourceType;
      ImGui::Text("%s", selectedAsset.name.data());
      if (!selectedAsset.resourceType->metaDataAsset)
      {
        if (adding)
        {
          static bool wantCopy = false;
          static Asset<AssetStub> stub;
          static const char *copyName;
          ImGui::SameLine();
          if (ImGui::Button("Copy"))
            wantCopy = !wantCopy, stub = Asset<AssetStub>();
          if (wantCopy)
          {
            static string searchString = "";
            snprintf(buf, BUFN, "%s", searchString.c_str());
            if (ImGui::InputText("Search substr", buf, BUFN))
            {
              searchString.assign(buf);
            }
            static int curCopy = -1;
            eastl::vector<const char *, tmp_allocator> names;
            
            for (auto &asset : selectedAsset.resourceType->resources)
            {
              const string &assetName = asset.second.asset_name();
              if (strstr(assetName.c_str(), searchString.c_str()))
                names.push_back(assetName.c_str());
            }
            if (ImGui::ListBox("", &curCopy, names.data(), names.size()))
            {
              stub = selectedAsset.resourceType->resources[string(names[curCopy])];
              copyName = names[curCopy];
            }
          }
          ImGui::SameLine();
          snprintf(buf, BUFN, "%s%s", stub ? "Copy " : "Create", stub ?  copyName : "");
          std::filesystem::path path;
          if (ImGui::Button(buf) && get_save_file(path, "." + selectedAsset.resourceType->name))
          {
            if (stub)
              selectedAsset.resourceType->createCopyAsset(path, stub);
            else
              selectedAsset.resourceType->createNewAsset(path);
            adding = false;
          }
          
          //if (widgets.back.get())
          if (ImGui::Button("Back"))
          {
            adding = false;
            scroll = 0;
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
      if (ImGui::Button("Close viewer"))
      {
        selectedAsset.resourceType = nullptr;
        selectedAsset.asset = nullptr;
        adding = false;
        goto end;
      }
      if (!adding)
      {
        if (selectedAsset.asset)
        {
          //if (widgets.back.get())
          if (ImGui::Button("Back"))
          {
            selectedAsset.asset = nullptr;
            scroll *= -1;
          }
          else
          if (selectedAsset.asset->loaded() && selectedAsset.resourceType->editAsset(*selectedAsset.asset))
            selectedAsset.resourceType->reloadAsset(*selectedAsset.asset);
        }
        else
        {
          for (auto &asset : selectedAsset.resourceType->resources)
          {
            const string &assetName = asset.second.asset_name();
            if (ImGui::Button(assetName.c_str()))
            {
              selectedAsset.asset = &asset.second;
              selectedAsset.resourceType->loadAsset(asset.second);
            }
          }
          if (scroll < 0)
          {
            ImGui::SetScrollY(-scroll);
            scroll *= -1;
          }
          else
            scroll = ImGui::GetScrollY();
        }
      }
    }
end:
    ImGui::End();
  }
}
