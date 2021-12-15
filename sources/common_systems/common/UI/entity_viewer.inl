#include <ecs.h>
#include <imgui.h>
#include <manager/entity_container.h>
#include <functional>
#include "editor_window.h"

constexpr uint lockFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
EVENT(ecs::SystemTag::GameEditor,ecs::SystemTag::Debug) init_imgui_style(const ecs::OnSceneCreated&, EditorUI &ui)
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.IndentSpacing = 5;
  //style.ItemSpacing = ImVec2(0, 0);
  //style.ItemInnerSpacing = ImVec2(0, 0);
  ui.windowFlags = lockFlag;

}
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::GameEditor,ecs::SystemTag::Debug) entity_viewer(const EditorUI &ui)
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
      for (int j = 0; j < archetype->count; ++j)
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
            const ecs::TypeInfo &typeInfo = ecs::TypeInfo::types()[full_descr->typeHash];
            snprintf(buf, N, "%s %s",  full_descr->name.c_str(), typeInfo.name.c_str());
            if (ImGui::TreeNode(buf))
            {
              bool edited = typeInfo.componentEdition(archetype->components[full_descr->hash].get_component<void>(j), false);
              if (edited)
                ecs::send_event(eid, ecs::OnEntityEdited());
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
