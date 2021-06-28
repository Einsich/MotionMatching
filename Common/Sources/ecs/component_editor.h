#pragma once
#include "ecs/manager/is_vector.h"
#include "Engine/imgui/imgui.h"
#include "Serialization/reflection.h"

template<typename T>
std::enable_if_t<HasReflection<T>::value, void> edit_component(T &component, const char *)
{
  component.reflect([](auto &component, const char *name)->void{edit_component(component, name);});
}

template<typename T>
std::enable_if_t<!HasReflection<T>::value && !is_vector<T>::value, void>
edit_component(T &component, const char *name);

template<typename T>
std::enable_if_t<is_vector<T>::value, void>
edit_component(T &v, const char *name)
{
  std::vector<typename T::value_type> &component = v;
  constexpr int BUFN = 255;
  char buf[BUFN];
  snprintf(buf, BUFN, "%s##%p", name, (void*)&component);
  if (ImGui::TreeNode(buf))
  {
    ImGui::SameLine();
    int count = component.size();
    snprintf(buf, BUFN, "count##%p", (void*)&component);
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      component.resize(count);
    }
    for (uint i = 0; i < component.size(); ++i)
    {
      snprintf(buf, BUFN, "[%u]##%p", i, (void*)&component);
      if (ImGui::TreeNode(buf))
      {
        snprintf(buf, BUFN, "##%p", (void*)&component);
        if constexpr (std::is_same_v<bool, typename T::value_type>)
        {
          bool b = component[i];
          edit_component(b, buf);
          b = component[i];
        }
        else
        {
          edit_component(component[i], buf);
        }
        ImGui::TreePop();  
      }
      ImGui::SameLine();
      snprintf(buf, BUFN, "remove##%d%p", i, (void*)&component);
      if (ImGui::Button(buf))
      {
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
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))\
    {
      component.resize(count);
    }
  }
}

#define EDIT_STUB(T) \
template<>\
void edit_component(T &, const char *name)\
{\
  ImGui::Text("%s", name);\
  ImGui::Spacing();\
}
