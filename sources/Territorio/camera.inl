#include <ecs.h>
#include <transform2d.h>
#include <input.h>
#include <application/time.h>
#include "world_render.h"

EVENT() setup_camera(
  const ecs::OnEntityCreated &,
  mat3 &cameraProjection,
  vec3 &zoom,
  float pixelPerUnit,
  float minZoom,
  float maxZoom,
  float curZoom,
  Transform2D &transform,
  Transform2D &targetTransform,
  WorldRenderer &wr)
{
  //camera creation
  auto [width, height] = get_resolution();
  float orthoScale = 1.f / pixelPerUnit;
  cameraProjection = mat3(0.f);
  cameraProjection[0][0] = 1.f / width;
  cameraProjection[1][1] = 1.f / height;

  zoom = vec3(orthoScale * minZoom, orthoScale * maxZoom, curZoom);
  targetTransform = transform = Transform2D(vec2(0), vec2(orthoScale * curZoom));
  wr.screenToWorld = transform.get_matrix();
  wr.resolution = ivec2(width, height);
}

SYSTEM() camera_lerp(
  const Transform2D &targetTransform,
  Transform2D &transform,
  float lerpStrength
)
{
  float t = lerpStrength * Time::delta_time();
  transform.position = glm::lerp(transform.position, targetTransform.position, t);
  transform.scale = glm::lerp(transform.scale, targetTransform.scale, t);
}

EVENT(mat3 cameraProjection) change_zoom(
  const MouseWheelEvent &event,
  Transform2D &targetTransform,
  float zoomStrength,
  vec3 &zoom)
{
  if (Input::input().mouseOverUI)
    return;
  zoom.z = glm::clamp(zoom.z - event.wheel * zoomStrength * zoom.z, 0.f, 1.f);
  targetTransform.scale = vec2(lerp(zoom.x, zoom.y, zoom.z));
}

EVENT(mat3 cameraProjection) move_camera(
  const MouseMoveEvent &event,
  Transform2D &targetTransform,
  const WorldRenderer &wr,
  bool cameraLocked)
{
  if (Input::input().mouseOverUI)
    return;
  if (!cameraLocked)
  {
    vec3 d = wr.screenToWorld * vec3(event.dx, -event.dy, 0);
    targetTransform.position -= vec2(d.x, d.y)*2.f;
  }
}

EVENT(mat3 cameraProjection) lock_unlock_camera(
  const MouseButtonDownEvent<MouseButton::LeftButton> &event,
  bool &cameraLocked)
{
  if (Input::input().mouseOverUI)
    return;
  cameraLocked = event.action == MouseAction::Up;
}
