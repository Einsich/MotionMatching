#include <ecs/ecs.h>
#include <ecs/archetype_manager.h>
#include <imgui.h>

#include <functional>
#include "editor_window.h"
#include <ecs/editor_events.h>
#include <ecs/imgui.h>
#include <ecs/user_type_info.h>

constexpr uint lockFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
EVENT(tags=debug) init_imgui_style(const ecs::OnSceneCreated&, EditorUI &ui)
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.IndentSpacing = 5;
  //style.ItemSpacing = ImVec2(0, 0);
  //style.ItemInnerSpacing = ImVec2(0, 0);
  ui.windowFlags = lockFlag;

}
EVENT(tags=debug) entity_viewer(const ImguiRender&, const EditorUI &ui)
{
  if(!ImGui::Begin("Entity viewer", nullptr, ui.windowFlags))
  {
    ImGui::End();
    return;
  }
  ImGui::PushItemWidth(170);
  const auto &types = ecs::get_all_registered_types();
  const auto &userTypes = ecs::get_all_user_info<UserFunctions>();

  const int N = 100;
  char buf[N];
  int archetypeIndex = 0;
  for (auto &archetype : ecs::get_archetype_manager().archetypes)
  {

    for (int j = 0, n = std::min(archetype.entityCount, 200u); j < n; ++j)
    {
      //ecs::EntityId eid = ecs::find_entity(archetypeIndex, j);
      ecs::uint chunk, offset;
      archetype.get_chunk_offset(j, chunk, offset);
      snprintf(buf, N, "entity[%d][%d]", archetypeIndex, j);
      if (ImGui::TreeNode(buf))
      {
        ImGui::SameLine();
        if (ImGui::Button("destroy"))
        {
          archetype.destroy_entity(j);
        }
        else
        for (const auto&cmp : archetype.components)
        {
          const auto &typeInfo = types[cmp.description.typeIndex];
          snprintf(buf, N, "%s %s", cmp.description.name.c_str(), typeInfo.name.c_str());
          if (ImGui::TreeNode(buf))
          {
            bool edited = userTypes[cmp.description.typeIndex].componentEdition(cmp.data[chunk] + offset * typeInfo.sizeOf, false);
            if (edited)
            {
              // ecs::send_event(eid, OnEntityEdited());
            } 
            ImGui::TreePop();
          }
        }
        ImGui::TreePop();
      }
    }
    ++archetypeIndex;
  }
  ImGui::End();
}
