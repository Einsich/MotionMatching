#include "ecs/ecs.h"
#include "Engine/imgui/imgui.h"
#include <functional>
SYSTEM(ecs::SystemOrder::UI) entity_viewer()
{
  if(!ImGui::Begin("Entity viewer"))
  {
    ImGui::End();
    return;
  }
  int i = 0;
  const int N = 100;
  char buf[N];
  for (ecs::Archetype *archetype : ecs::core().archetypes)
  {
    snprintf(buf, N, "archetype[%d]", ++i);
    if (ImGui::TreeNode(buf))
    {
      for (uint j = 0; j < archetype->count; ++j)
      {
        snprintf(buf, N, "entity[%d]", j);
        if (ImGui::TreeNode(buf))
        {
          for (const ecs::FullTypeDescription *full_descr : archetype->fullTypeDescriptions)
          {
            snprintf(buf, N, "%s %s",  full_descr->name.c_str(), full_descr->type.c_str());
            if (ImGui::TreeNode(buf))
            {
              full_descr->componentEdition(archetype->components[full_descr->hash].get_component<void>(j));
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
  ImGui::End();
}