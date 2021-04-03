#include "camera.h"
#include <vector>
#include "Application/application.h"

static CameraPtr mainCamera = nullptr;
static vector<CameraPtr> cameras;


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
Transform& Camera::get_transform()
{
  return transform;
}
mat4x4 Camera::get_transform_matrix() const
{
  return transform.get_transform();
}

void Camera::set_to_shader(const Shader& shader, bool sky_box) const
{
  vec3 cameraPosition = transform.get_position();
  mat4 view = inverse(transform.get_transform());

  mat4x4 viewProjection = projection * (sky_box ? mat4(mat3(view)): view);
  shader.set_mat4x4("ViewProjection", viewProjection);
  shader.set_vec3("CameraPosition", cameraPosition);
}
void Camera::change_main_cam(CameraPtr camera)
{
  if (mainCamera)
    mainCamera->isMainCamera = false;
  mainCamera = camera;
  mainCamera->isMainCamera = true;
}
void Camera::add_camera(CameraPtr camera, bool is_main)
{
  if (is_main)
  {
    change_main_cam(camera);
  }
  cameras.emplace_back(camera);
}

void Camera::set_main_camera(CameraPtr camera)
{
  if (std::find(cameras.begin(), cameras.end(), camera) != cameras.end())
  {
    change_main_cam(camera);
  }
  else
    debug_error("Can't set unregistred camera to main Camera");
}
void Camera::set_next_camera()
{
  auto it = std::find(cameras.begin(), cameras.end(), mainCamera);
  if (it != cameras.end())
  {
    int i = it - cameras.begin();
    i = (i + 1) % cameras.size();
    change_main_cam(cameras[i]);
  }
  else
    debug_error("Can't set next camera to main Camera");
}
CameraPtr Camera::main_camera()
{
  return mainCamera;
}