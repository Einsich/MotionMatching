#include "camera.h"
#include <vector>
#include "application/application_data.h"
#include "component_editor.h"
#include "imgui.h"

Camera::Camera():
orthographic(false),
orhtoScale(1.f),
aspectRatio(Application::get_context().get_aspect_ratio()),
fieldOfView(90), 
zNear(0.1f),
zFar(1000.f)
{
  projection = perspective(fieldOfView * DegToRad, aspectRatio, zNear, zFar);
}

void Camera::set_orthographic(float aspectRatio, float orhtoScale, float zNear, float zFar) 
{ 
  orthographic = true;
  this->aspectRatio = aspectRatio;
  this->zFar = zFar;
  this->zNear = zNear;
  projection = ortho(-orhtoScale, orhtoScale, -orhtoScale * aspectRatio, orhtoScale * aspectRatio, zNear, zFar); 
}

void Camera::set_perspective(float fieldOfView, float zNear, float zFar)
{
  orthographic = false;
  this->fieldOfView = fieldOfView;
  this->zFar = zFar;
  this->zNear = zNear;
  const float aspectRatio = Application::get_context().get_aspect_ratio();
  projection = perspective(fieldOfView * DegToRad, aspectRatio, zNear, zFar);
}
const mat4x4& Camera::get_projection() const
{
  return projection;
}

bool edit_component(Camera &camera, const char *name, bool view_only)
{
  ImGuiInputTextFlags flags = view_only ? ImGuiInputTextFlags_ReadOnly : 0;
  bool edited = false;
  ImGui::Text("%s", name);
  bool b = camera.orthographic;
  edited |= ImGui::Checkbox("orthographic", view_only ? &b : &camera.orthographic);
  if (camera.orthographic)
  {
    edited |= ImGui::InputFloat("aspectRatio", &camera.aspectRatio, 0.1f, 10, 2, flags);
    edited |= ImGui::InputFloat("orhtoScale", &camera.orhtoScale, 0.1f, 10, 2, flags);
    edited |= ImGui::InputFloat("zNear", &camera.zNear, 0.01f, 10, 3, flags);
    edited |= ImGui::InputFloat("zFar", &camera.zFar, 100.f, 10, 1, flags);
    if (edited)
      camera.set_orthographic(camera.aspectRatio, camera.orhtoScale, camera.zNear, camera.zFar);
  }
  else
  {
    edited |= ImGui::InputFloat("fieldOfView", &camera.fieldOfView, 10.f, 10, 2, flags);
    edited |= ImGui::InputFloat("zNear", &camera.zNear, 0.01f, 10, 3, flags);
    edited |= ImGui::InputFloat("zFar", &camera.zFar, 100.f, 10, 1, flags);
    if (edited)
      camera.set_perspective(camera.fieldOfView, camera.zNear, camera.zFar);
  }
  ImGui::Spacing();
  return edited;
}

ArcballCamera::ArcballCamera(float distance, vec2 rotation, vec3 target):
  curZoom(0.2f),
  maxdistance(distance),
  targetZoom(curZoom),
  distance(curZoom * maxdistance),
  lerpStrength(10),
  mouseSensitivity(0.5f),
  wheelSensitivity(0.05f),
  targetRotation(rotation),
  targetPosition(target),
  rotationEnable(false),
  curRotation(rotation)
{}
void ArcballCamera::set_target(vec3 target)
{
  targetPosition = target;
}
void ArcballCamera::calculate_transform(Transform &transform)
{
  float y = curRotation.y;
  float x = curRotation.x ;
  vec3 direction = vec3(cos(x) * cos(y), sin(y), sin(x) * cos(y));
  
  transform.set_position(targetPosition - distance * direction);
  transform.set_rotation(PI*0.5-curRotation.x, -curRotation.y, 0);
}
FreeCamera::FreeCamera():
  curRotation(0.f),
  wantedRotation(0.f),
  curPosition(0.f),
  wantedPosition(0.f),
  rotationable(false),
  screenSpaceMovable(false),
  minSpeed(5),
  maxSpeed(50),
  lerpStrength(10),
  rotationSensitivity(0.3),
  screenMoveSensitivity(0.1)
{}
void FreeCamera::calculate_transform(Transform &transform)
{
  transform.set_rotation(curRotation.x, curRotation.y, 0);
}