#pragma once
#include "Engine/imgui/imgui.h"

template<typename T>
void edit_component(T &component, const char *name);


#define EDIT_VECTOR(T) template<>\
void edit_component(std::vector<T> &component, const char *name)\
{\
  constexpr int BUFN = 255; \
  char buf[BUFN]; \
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