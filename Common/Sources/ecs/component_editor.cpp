
#include "component_editor.h"
#include "ecs/manager/entity_id.h"
#include "Engine/camera.h"
#include "Engine/Render/skybox.h"
#include "ecs/base_types.h"
#include "Serialization/reflection.h"

constexpr int BUFN = 255;
char buf[BUFN];



template<>
void edit_component(bool &component, const char *name)
{
  ImGui::Checkbox(name, &component);
  ImGui::Spacing();
}
template<>
void edit_component(int &component, const char *name)
{
  ImGui::InputInt(name, &component);
  ImGui::Spacing();
}
template<>
void edit_component(uint &component, const char *name)
{
  int v = component;
  ImGui::InputInt(name, &v);
  component = v;
  ImGui::Spacing();
}
template<>
void edit_component(float &component, const char *name)
{
  ImGui::InputFloat(name, &component, 0.1f, 10, 2);
  ImGui::Spacing();
}

template<>
void edit_component(double &component, const char *name)
{
  ImGui::InputDouble(name, &component, 0.1f, 10);
  ImGui::Spacing();
}
#define VEC_TYPE(TYPE, INPUT)\
template<> void edit_component(TYPE &component, const char *name)\
{\
  ImGui::INPUT(name, &component.x);\
  ImGui::Spacing();\
}
VEC_TYPE(ivec2, InputInt2)
VEC_TYPE(ivec3, InputInt3)
VEC_TYPE(ivec4, InputInt4)

VEC_TYPE(vec2, InputFloat2)
VEC_TYPE(vec3, InputFloat3)
VEC_TYPE(vec4, InputFloat4)


template<>
void edit_component(mat4 &component, const char *name)
{
  vec3 rotation;
  glm::extractEulerAngleXYZ(component, rotation.x, rotation.y, rotation.z);
  rotation = glm::degrees(rotation);
  ImGui::InputFloat3(name, &rotation.x, 2);
  rotation = glm::radians(rotation);
  component = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
  ImGui::Spacing();
}
template<>
void edit_component(mat3 &component, const char *name)
{
  vec3 rotation;
  glm::extractEulerAngleXYZ((mat4)component, rotation.x, rotation.y, rotation.z);
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
template<>
void edit_component(std::string &component, const char *name)
{
  if (ImGui::InputText(name, buf, BUFN))
    component = std::string(buf);
  ImGui::Spacing();
}

