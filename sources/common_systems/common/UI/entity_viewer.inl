#include <ecs.h>
#include <imgui.h>
#include <manager/entity_container.h>
#include <functional>
#include "editor_window.h"
#include <ecs/editor_events.h>

constexpr uint lockFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
EVENT(scene=game, editor,scene=debug) init_imgui_style(const ecs::OnSceneCreated&, EditorUI &ui)
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.IndentSpacing = 5;
  //style.ItemSpacing = ImVec2(0, 0);
  //style.ItemInnerSpacing = ImVec2(0, 0);
  ui.windowFlags = lockFlag;

}
SYSTEM(stage=ui;scene=game, editor,scene=debug) entity_viewer(const EditorUI &ui)
{
  if(!ImGui::Begin("Entity viewer", nullptr, ui.windowFlags))
  {
    ImGui::End();
    return;
  }
  ImGui::PushItemWidth(170);
  const int N = 100;
  char buf[N];
  int archetypeIndex = 0;
  for (ecs::Archetype *archetype : ecs::core().entityContainer->archetypes)
  {
    if (ImGui::TreeNode(archetype->synonim.c_str()))
    {
      for (int j = 0, n = std::min(archetype->count, 200); j < n; ++j)
      {
        ecs::EntityId eid = ecs::find_entity(archetypeIndex, j);
        snprintf(buf, N, "entity[%d]", j);
        if (ImGui::TreeNode(buf))
        {
          ImGui::SameLine();
          if (ImGui::Button("destroy"))
          {
            archetype->destroy_entity(j, true);
          }
          else
          for (const ecs::FullTypeDescription *full_descr : archetype->fullTypeDescriptions)
          {
            const ecs::TypeInfo &typeInfo = *ecs::TypeInfo::types()[full_descr->typeHash];
            snprintf(buf, N, "%s %s",  full_descr->name.c_str(), typeInfo.name.c_str());
            if (ImGui::TreeNode(buf))
            {
              bool edited = typeInfo.userInfo.componentEdition(archetype->components[full_descr->hash].get_component<void>(j), false);
              if (edited)
                ecs::send_event(eid, OnEntityEdited());
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
    ++archetypeIndex;
  }
  ImGui::End();
}
