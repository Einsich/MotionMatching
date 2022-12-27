
#include "component_editor.h"
#include <ecs/entity_id.h>
#include <eastl/string.h>
#include <ecs/event_registration.h>
#include "serialization/reflection.h"
#include "3dmath.h"
constexpr int BUFN = 255;
char buf[BUFN];

#include <ecs/event.h>
#include "editor_events.h"
#include <imgui.h>

ECS_EVENT_REGISTRATION(OnEntityEdited)

bool edit_component(bool &component, const char *name, bool view_only)
{
  bool b = component;
  bool edited = ImGui::Checkbox(name, view_only ? &b : &component);
  ImGui::Spacing();
  return view_only ? false : edited;
}

bool edit_component(int &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  bool edited = ImGui::InputInt(name, &component, flags);
  ImGui::Spacing();
  return edited;
}

bool edit_component(uint &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  int v = component;
  bool edited = ImGui::InputInt(name, &v, flags);
  component = v;
  ImGui::Spacing();
  return edited;
}

bool edit_component(float &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  bool edited = ImGui::InputFloat(name, &component, 0.1f, 10, 2, flags);
  ImGui::Spacing();
  return edited;
}

bool edit_component(double &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  bool edited = ImGui::InputDouble(name, &component, 0.1f, 10, "%.6f", flags);
  ImGui::Spacing();
  return edited;
}
#define VEC_ITYPE(TYPE, INPUT)                                                \
  bool edit_component(TYPE &component, const char *name, bool view_only)      \
  {                                                                           \
    ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0; \
    bool edited = ImGui::INPUT(name, &component.x, flags);                    \
    ImGui::Spacing();                                                         \
    return edited;                                                            \
  }
#define VEC_UTYPE(TYPE, INPUT)                                                \
  bool edit_component(TYPE &component, const char *name, bool view_only)      \
  {                                                                           \
    ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0; \
    bool edited = ImGui::INPUT(name, (int *)&component.x, flags);             \
    ImGui::Spacing();                                                         \
    return edited;                                                            \
  }
#define VEC_TYPE(TYPE, INPUT)                                                 \
  bool edit_component(TYPE &component, const char *name, bool view_only)      \
  {                                                                           \
    ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0; \
    bool edited = ImGui::INPUT(name, &component.x, "%.2f", flags);            \
    ImGui::Spacing();                                                         \
    return edited;                                                            \
  }
VEC_ITYPE(ivec2, InputInt2)
VEC_ITYPE(ivec3, InputInt3)
VEC_ITYPE(ivec4, InputInt4)

VEC_UTYPE(uvec2, InputInt2)
VEC_UTYPE(uvec3, InputInt3)
VEC_UTYPE(uvec4, InputInt4)

VEC_TYPE(vec2, InputFloat2)
VEC_TYPE(vec3, InputFloat3)
VEC_TYPE(vec4, InputFloat4)

bool edit_component(mat4 &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  vec3 rotation;
  glm::extractEulerAngleXYZ(component, rotation.x, rotation.y, rotation.z);
  rotation = glm::degrees(rotation);
  bool edited = ImGui::InputFloat3(name, &rotation.x, 2, flags);
  if (edited)
  {
    rotation = glm::radians(rotation);
    component = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
  }
  ImGui::Spacing();
  return edited;
}

bool edit_component(mat3 &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  vec3 rotation;
  glm::extractEulerAngleXYZ((mat4)component, rotation.x, rotation.y, rotation.z);
  rotation = glm::degrees(rotation);
  bool edited = ImGui::InputFloat3(name, &rotation.x, 2, flags);
  if (edited)
  {
    rotation = glm::radians(rotation);
    component = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
  }
  ImGui::Spacing();
  return edited;
}

bool edit_component(mat2 &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;

  bool edited = ImGui::InputFloat2(name, &component[0].x, 2, flags);
  edited = ImGui::InputFloat2(name, &component[1].x, 2, flags) || edited;
  ImGui::Spacing();
  return edited;
}

bool edit_component(ecs::EntityId &component, const char *name, bool view_only)
{
  ImGui::Text("%s [%s] archetype %d, index %d", name, component ? "valid" : "not valid",
              component.description->archetype, component.description->index);
  bool edited = false;
  if (!view_only && strcmp(name, "eid"))
  {
    /*     int archIndex[2] = {component.archetype_index(), (int)component.array_index()};
        edited = ImGui::InputInt2("archetype&array index", archIndex);
        if (edited)
          component = ecs::entityManager->entityPull.find_entity(archIndex[0], archIndex[1]); */
  }
  ImGui::Spacing();
  return edited;
}

bool edit_component(eastl::string &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  snprintf(buf, BUFN, "%s", component.c_str());
  bool edited = ImGui::InputText(name, buf, BUFN, flags);
  if (edited)
    component = eastl::string(buf);
  ImGui::Spacing();
  return edited;
}
bool edit_component(std::string &component, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  snprintf(buf, BUFN, "%s", component.c_str());
  bool edited = ImGui::InputText(name, buf, BUFN, flags);
  if (edited)
    component = std::string(buf);
  ImGui::Spacing();
  return edited;
}
