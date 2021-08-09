#pragma once
#include "3dmath.h"
#include "transform.h"
#include "ecs/ecs.h"
#include "Serialization/reflection.h"
struct Camera
{
private:
  mat4x4 projection;
public:
  bool orthographic;
  float orhtoScale, aspectRatio, fieldOfView, zNear, zFar;
  Camera();
  void set_orthographic(float aspectRatio, float orhtoScale, float zNear, float zFar);
  void set_perspective(float fieldOfView, float zNear, float zFar);
  const mat4x4& get_projection() const;
};


struct ArcballCamera
{
public: 
  float curZoom;
  REFLECT(
  ArcballCamera,
  (float) (maxdistance),
  (float) (targetZoom), 
  (float) (distance), 
  (float) (lerpStrength), 
  (float) (mouseSensitivity), 
  (float) (wheelSensitivity), 
  (vec2) (targetRotation),
  (vec3) (targetPosition)
  )
  bool rotationEnable;
  vec2 curRotation;
  ArcballCamera(float distance, vec2 rotation, vec3 target = vec3());
  ArcballCamera() = default;
  void set_target(vec3 target);
  void calculate_transform(Transform &transform);
};
struct FreeCamera
{
public: 
  vec2 curRotation;
  vec2 wantedRotation;
  vec3 curPosition;
  vec3 wantedPosition;
  bool rotationable, screenSpaceMovable;
  REFLECT(
  FreeCamera,
  (float) (minSpeed),
  (float) (maxSpeed),
  (float) (lerpStrength),
  (float) (rotationSensitivity),
  (float) (screenMoveSensitivity)
)
  FreeCamera();
  void calculate_transform(Transform &transform);
};
struct OnSetMainCamera
{
  ecs::EntityId mainCamera;
  OnSetMainCamera(ecs::EntityId camera):
    mainCamera(camera){}
};

bool main_camera(mat4 &cam_transform, mat4 &cam_projection);
vec3 main_camera_position();


ecs::EntityId create_arcball_camera(float dist, vec2 rotation, vec3 target);
ecs::EntityId create_arcball_camera(float dist, vec2 rotation, ecs::EntityId target);

ecs::EntityId create_free_camera(vec3 position, vec2 rotation);
ecs::EntityId create_camera(vec3 position = vec3(0.f), vec2 rotation = vec2(0.f));
