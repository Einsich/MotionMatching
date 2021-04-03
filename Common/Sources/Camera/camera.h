#pragma once
#include "common.h"
#include "math.h"
#include "Transform/transform.h"
#include "Event/input.h"
#include "Shader/shader.h"
#include "component.h"
class Camera : public Component
{
protected:
  Transform transform;
  mat4x4 projection;
  bool isMainCamera;
public:
  void set_orthographic(float width, float height, float zNear, float zFar);
  void set_perspective(float fieldOfView, float aspectRatio, float zNear, float zFar);
  void set_perspective(float fieldOfView, float zNear, float zFar);

  const mat4x4& get_projection() const;
  Transform& get_transform();
  mat4x4 get_transform_matrix() const;
  void set_to_shader(const Shader& shader, bool sky_box = false) const;
typedef shared_ptr<Camera> CameraPtr;
  static void add_camera(CameraPtr camera, bool is_main = false);
  static void change_main_cam(CameraPtr camera);
  static void set_main_camera(CameraPtr camera);
  static void set_next_camera();
  static CameraPtr  main_camera(); 
};

typedef shared_ptr<Camera> CameraPtr;

