#pragma once
#include "Engine/imgui/imgui.h"

template<typename T>
void edit_component(T &component, const char *name);


#define EDIT_VECTOR(Type) \
template<>\
void edit_component(std::vector<Type> &component, const char *name)\
{\
  snprintf(buf, BUFN, "%s [%d]", name, (int)component.size());\
  if (ImGui::TreeNode(buf))\
  {\
    for (uint i = 0; i < component.size(); ++i)\
    {\
      snprintf(buf, BUFN, "[%u]", i);\
      edit_component(component[i], buf);\
    }\
    ImGui::TreePop();  \
    ImGui::Spacing();\
  }\
}
#define EDIT_STUB(Type) \
template<>\
void edit_component(Type &, const char *name)\
{\
  ImGui::Text("%s", name);\
  ImGui::Spacing();\
}