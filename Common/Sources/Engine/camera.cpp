#include "camera.h"
#include <vector>
#include "Application/application.h"


void Camera::set_orthographic(float width, float height, float zNear, float zFar) 
{ 
  const float aspectRatio = width / height;
  projection = ortho(-1.f, 1.f,-1.f * aspectRatio, 1.f * aspectRatio, zNear, zFar); 
}
void Camera::set_perspective(float fieldOfView, float aspectRatio, float zNear, float zFar)
{ 
  projection = perspective(fieldOfView, aspectRatio, zNear, zFar);
}

void Camera::set_perspective(float fieldOfView, float zNear, float zFar)
{
  const float aspectRatio = (float)Application::get_context().get_width() / Application::get_context().get_height();
  projection = perspective(fieldOfView, aspectRatio, zNear, zFar);
}
const mat4x4& Camera::get_projection() const
{
  return projection;
}

ArcballCamera::ArcballCamera(float distance, vec2 rotation, vec3 target):
  maxdistance(distance),
  zoom(0.2f),
  targetZoom(zoom),
  distance(zoom * maxdistance),
  rotation(rotation),
  targetRotation(rotation),
  target_position(target),
  rotationEnable(false)
{}
void ArcballCamera::set_target(vec3 target)
{
  target_position = target;
}
void ArcballCamera::calculate_transform(Transform &transform)
{
  float y = rotation.y;
  float x = rotation.x ;
  vec3 direction = vec3(cos(x) * cos(y), sin(y), sin(x) * cos(y));
  
  transform.set_position(target_position - distance * direction);
  transform.set_rotation(PI*0.5-rotation.x, -rotation.y, 0);
}
FreeCamera::FreeCamera(vec3 position, vec2 rotation):
  curRotation(rotation),
  wantedRotation(rotation),
  curPosition(position),
  wantedPosition(position),
  rotationEnable(false)
{}
void FreeCamera::calculate_transform(Transform &transform)
{
  transform.set_rotation(PI*0.5f-curRotation.x, -curRotation.y, 0);
}

void set_camera_to_shader(const Shader& shader, const mat4 &viewProjection, const vec3 &cameraPosition)
{
  shader.set_mat4x4("ViewProjection", viewProjection);
  shader.set_vec3("CameraPosition", cameraPosition);
}