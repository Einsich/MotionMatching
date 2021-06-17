#include "component_editor.h"
#include "3dmath.h"
#include "ecs/manager/entity_id.h"
#include "Engine/camera.h"
constexpr int BUFN = 100;
char buf[BUFN];

template<>
void edit_component(bool &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::Checkbox(name, &component);
  ImGui::Spacing();
}
template<>
void edit_component(int &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::InputInt(name, &component);
  ImGui::Spacing();
}
template<>
void edit_component(float &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::InputFloat(name, &component, 0.1f, 10, 2);
  ImGui::Spacing();
}
template<>
void edit_component(vec2 &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::InputFloat2(name, &component.x, 2);
  ImGui::Spacing();
}
template<>
void edit_component(vec3 &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::InputFloat3(name, &component.x, 2);
  ImGui::Spacing();
}
template<>
void edit_component(vec4 &component, const char *name)
{
  ImGui::Text("%s", name);
  ImGui::InputFloat4(name, &component.x, 2);
  ImGui::Spacing();
}
template<>
void edit_component(mat4 &component, const char *name)
{
  ImGui::Text("%s", name);
  vec3 rotation;
  glm::extractEulerAngleXYZ(component, rotation.x, rotation.y, rotation.z);
  rotation = glm::degrees(rotation);
  ImGui::InputFloat3(name, &rotation.x, 2);
  rotation = glm::radians(rotation);
  component = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
  ImGui::Spacing();
}
template<>
void edit_component(ecs::EntityId &component, const char *name)
{
  ImGui::Text("%s [%s] archetype %d, index %d", name,component.valid() ? "valid":"not valid",
   component.archetype_index(), component.array_index());
  ImGui::Spacing();
}
EDIT_VECTOR(ecs::EntityId)

EDIT_STUB(Camera)

