#pragma once
#include "Engine/imgui/imgui.h"

template<typename T>
void edit_component(T &component, const char *name);


#define EDIT_VECTOR(T) template<>\
void edit_component(std::vector<T> &component, const char *name)\
{\
  constexpr int BUFN = 255;\
  char buf[BUFN];\
  snprintf(buf, BUFN, "%s##%p", name, (void*)&component);\
  if (ImGui::TreeNode(buf))\
  {\
    ImGui::SameLine();\
    int count = component.size();\
    snprintf(buf, BUFN, "count##%p", (void*)&component);\
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))\
    {\
      component.resize(count);\
    }\
    for (uint i = 0; i < component.size(); ++i)\
    {\
      snprintf(buf, BUFN, "[%u]##%p", i, (void*)&component);\
      if (ImGui::TreeNode(buf))\
      {\
        snprintf(buf, BUFN, "##%p", (void*)&component);\
        edit_component(component[i], buf);\
        ImGui::TreePop();  \
      }\
      ImGui::SameLine();\
      snprintf(buf, BUFN, "remove##%d%p", i, (void*)&component);\
      if (ImGui::Button(buf))\
      {\
        component.erase(component.begin() + i);\
        --i;\
      }\
    }\
    ImGui::TreePop();  \
    ImGui::Spacing();\
  }\
  else\
  {\
    ImGui::SameLine();\
    int count = component.size();\
    snprintf(buf, BUFN, "count##%p", (void*)&component);\
    if (ImGui::InputInt(buf, &count, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))\
    {\
      component.resize(count);\
    }\
  }\
}

#define EDIT_STUB(T) \
template<>\
void edit_component(T &, const char *name)\
{\
  ImGui::Text("%s", name);\
  ImGui::Spacing();\
}

#define EDIT_REFLECTABLE(T)\
template<>\
void edit_component(T &component, const char *name)\
{\
  ImGui::Text("%s", name);\
  component.editor_reflect();\
}