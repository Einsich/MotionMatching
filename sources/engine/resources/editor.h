#pragma once
#include <functional>
#include "imgui.h"

template<typename T>
bool edit_vector(vector<T> &component, const char *name, std::function<bool(T&)> component_edit)
{
  constexpr int BUFN = 255;
  char buf[BUFN];
  snprintf(buf, BUFN, "%s##%p", name, (void*)&component);
  bool edited = false;
  if (ImGui::TreeNode(buf))
  {
    ImGui::SameLine();
    int count = component.size();
    snprintf(buf, BUFN, "count##%p", (void*)&component);
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      component.resize(count);
      edited |= true;
    }
    for (uint i = 0; i < component.size(); ++i)
    {
      snprintf(buf, BUFN, "[%u]##%p", i, (void*)&component);
      if (ImGui::TreeNode(buf))
      {
        snprintf(buf, BUFN, "##%p", (void*)&component);
        if constexpr (std::is_same_v<bool, T>)
        {
          bool b = component[i];
          edited |= component_edit(b);
          b = component[i];
        }
        else
        {
          edited |= component_edit(component[i]);
        }
        ImGui::TreePop();  
      }
      ImGui::SameLine();
      snprintf(buf, BUFN, "remove##%d%p", i, (void*)&component);
      if (ImGui::Button(buf))
      {
        edited |= true;
        component.erase(component.begin() + i);
        --i;
      }
    }
    ImGui::TreePop();  
    ImGui::Spacing();
  }
  else
  {
    ImGui::SameLine();
    int count = component.size();
    snprintf(buf, BUFN, "count##%p", (void*)&component);
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      edited |= true;
      component.resize(count);
    }
  }
  return edited;
}
