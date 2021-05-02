#pragma once
#include "3dmath.h"
#include "Render/Shader/shader.h"
#include "transform.h"
#include "ecs/ecs.h"
struct Camera
{
private:
  mat4x4 projection;

public:
  void set_orthographic(float width, float height, float zNear, float zFar);
  void set_perspective(float fieldOfView, float aspectRatio, float zNear, float zFar);
  void set_perspective(float fieldOfView, float zNear, float zFar);

  const mat4x4& get_projection() const;
};


struct ArcballCamera
{
public: 
  float maxdistance, zoom, targetZoom, distance;
  vec2 rotation, targetRotation;
  vec3 target_position;
  bool rotationEnable;
  ArcballCamera(float distance, vec2 rotation, vec3 target = vec3());
  void set_target(vec3 target);
  void calculate_transform(Transform &transform);
};
struct FreeCamera
{
public: 
  vec2 curRotation, wantedRotation;
  vec3 curPosition, wantedPosition;
  bool rotationEnable;
  FreeCamera(vec3 position, vec2 rotation);
  void calculate_transform(Transform &transform);
};
struct OnSetMainCamera
{
  ecs::EntityId mainCamera;
  OnSetMainCamera(ecs::EntityId camera):
    mainCamera(camera){}
};

void set_camera_to_shader(const Shader& shader, const mat4 &viewProjection, const vec3 &cameraPosition);
bool main_camera(mat4 &cam_transform, mat4 &cam_projection);

ecs::EntityId create_camera_manager();

ecs::EntityId create_arcball_camera(float dist, vec2 rotation, vec3 target);
ecs::EntityId create_arcball_camera(float dist, vec2 rotation, ecs::EntityId target);

ecs::EntityId create_free_camera(vec3 position, vec2 rotation);
ecs::EntityId create_camera(vec3 position = vec3(0.f), vec2 rotation = vec2(0.f));
