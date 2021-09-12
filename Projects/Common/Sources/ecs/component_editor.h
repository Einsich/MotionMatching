#pragma once
#include "ecs/manager/is_vector.h"
#include "Engine/imgui/imgui.h"
#include "Serialization/reflection.h"
#include "Engine/Resources/asset.h"
template<typename T>
std::enable_if_t<HasReflection<T>::value, bool> edit_component(T &component, const char *, bool view_only)
{
  bool edited = false;
  component.reflect([&](auto &component, const char *name)->void{edited |= edit_component(component, name, view_only);});
  return edited;
}

template<typename T>
std::enable_if_t<!HasReflection<T>::value && !is_vector<T>::value, bool>
edit_component(T &component, const char *name, bool view_only);

template<typename T>
std::enable_if_t<is_vector<T>::value, bool>
edit_component(T &v, const char *name, bool view_only)
{
  std::vector<typename T::value_type> &component = v;
  constexpr int BUFN = 255;
  char buf[BUFN];
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  snprintf(buf, BUFN, "%s##%p", name, (void*)&component);
  bool edited = false;
  if (ImGui::TreeNode(buf))
  {
    ImGui::SameLine();
    int count = component.size();
    snprintf(buf, BUFN, "count##%p", (void*)&component);
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue | flags))
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
        if constexpr (std::is_same_v<bool, typename T::value_type>)
        {
          bool b = component[i];
          edited |= edit_component(b, buf, view_only);
          b = component[i];
        }
        else
        {
          edited |= edit_component(component[i], buf, view_only);
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
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue | flags))
    {
      edited |= true;
      component.resize(count);
    }
  }
  return edited;
}

template<typename T>
std::enable_if_t<is_base_of<IAsset, T>::value, bool>
 edit_component(Asset<T> &component, const char *, bool view_only)
{
  constexpr const string_view &tName = nameOf<T>::value;
  constexpr int BUFN = 255;
  char buf[BUFN];
  if (component)
    ImGui::Text("%s %s %s", tName.data(), component.asset_name().c_str(), component.is_copy() ? "(copy)" : "");
  else
    ImGui::Text("%s null", tName.data());

  bool edited = false;
  if (!view_only)
  {
    if (component && component.is_copy())
    {
      return component.edit();
    }
    else
    {
      static bool select = false;
      snprintf(buf, BUFN, "Select asset##%p", (void*)&component);
      if (ImGui::Button(buf))
        select = !select;
      if (select)
      {
        vector<const char *> names;
        const auto &resMap = Resources::instance().assets[tName];
        for (const auto &asset : resMap.resources)
        {
          names.push_back(asset.first.c_str());
        }
        static int curTex = -1;
        snprintf(buf, BUFN, "##%p", (void*)&component);
        if (ImGui::ListBox(buf, &curTex, names.data(), names.size(), 10))
        {
          select = false;
          auto it = resMap.resources.find(string(names[curTex]));
          if (it != resMap.resources.end())
          {
            component = it->second;
            edited = true;
          }
        }
      }
    }
  }
  return edited;
}
#define EDIT_STUB(T) \
template<>\
bool edit_component(T &, const char *name, bool)\
{\
  ImGui::Text("%s", name);\
  ImGui::Spacing();\
  return false; \
}
