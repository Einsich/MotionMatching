#pragma once
#include "utils/is_vector.h"
#include "imgui.h"
#include "serialization/reflection.h"
#include "resources/asset.h"
#include "resources/resources.h"
#include "manager/entity_id.h"
#include "utils/compile_time_string.h"

template<typename T>
std::enable_if_t<HasReflection<T>::value, bool> edit_component(T &component, const char *, bool view_only)
{
  bool edited = false;
  component.reflect([&](auto &component, const char *name)->void{edited |= edit_component(component, name, view_only);});
  return edited;
}


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
          if constexpr (requires(typename T::value_type &v) { edit_component(v, "", false); })
          {
            edited |= edit_component(component[i], buf, view_only);
          }
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
      static std::map<intptr_t, bool> selectionMap;
      intptr_t hash = (intptr_t)(void *)&component;
      bool &select = selectionMap[hash];
      snprintf(buf, BUFN, "Select asset##%p", (void*)&component);
      if (ImGui::Button(buf))
        select = !select;
      if (select)
      {
        vector<const char *> names;
        vector<const string *> keys;
        const auto &resMap = Resources::instance().assets[tName];
        for (const auto &asset : resMap.resources)
        {
          names.emplace_back(asset.second.asset_name().c_str());
          keys.emplace_back(&asset.first);
        }
        static int curTex = -1;
        snprintf(buf, BUFN, "##%p", (void*)&component);
        if (ImGui::ListBox(buf, &curTex, names.data(), names.size(), 10))
        {
          select = false;
          auto it = resMap.resources.find(*keys[curTex]);
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


bool edit_component(bool &component, const char *name, bool view_only);
bool edit_component(int &component, const char *name, bool view_only);
bool edit_component(uint &component, const char *name, bool view_only);
bool edit_component(float &component, const char *name, bool view_only);
bool edit_component(double &component, const char *name, bool view_only);
bool edit_component(ivec2 &component, const char *name, bool view_only);
bool edit_component(ivec3 &component, const char *name, bool view_only);
bool edit_component(ivec4 &component, const char *name, bool view_only);
bool edit_component(uvec2 &component, const char *name, bool view_only);
bool edit_component(uvec3 &component, const char *name, bool view_only);
bool edit_component(uvec4 &component, const char *name, bool view_only);
bool edit_component(vec2 &component, const char *name, bool view_only);
bool edit_component(vec3 &component, const char *name, bool view_only);
bool edit_component(vec4 &component, const char *name, bool view_only);
bool edit_component(mat2 &component, const char *name, bool view_only);
bool edit_component(mat3 &component, const char *name, bool view_only);
bool edit_component(mat4 &component, const char *name, bool view_only);
bool edit_component(ecs::EntityId &component, const char *name, bool view_only);
bool edit_component(std::string &component, const char *name, bool view_only);
