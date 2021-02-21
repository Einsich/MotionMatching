#pragma once
#include "CommonCode/component.h"
#include "CommonCode/Mesh/mesh.h"
#include "CommonCode/Shader/shader.h"
#include "CommonCode/Material/material.h"
#include "CommonCode/Camera/camera.h"
#include "CommonCode/Light/direction_light.h"
#include "CommonCode/Transform/transform.h"
class MeshRender : public Component, public IRenderable
{
private:
  MeshPtr mesh;
  MaterialPtr material;
  Shader shader;
public:
 MeshRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader);
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false);
  void render(const Transform &transform, const Camera& mainCam, const DirectionLight& light, bool wire_frame = false);
  MaterialPtr get_material() const;
};
ComponentPtr create_plane(bool create_uv = false);
ComponentPtr create_cube(bool create_uv = false);
ComponentPtr create_sphere(int detailed = 1, bool smooth = false, bool create_uv = false);