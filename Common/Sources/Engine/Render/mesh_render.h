#pragma once
#include "mesh.h"
#include "Shader/shader.h"
#include "material.h"
#include "direction_light.h"
#include "Engine/transform.h"
#include "Serialization/reflection.h"
class MeshRender
{
private:
  REFLECT(MeshRender,
  (Asset<Mesh>) (mesh),
  (Asset<Material>) (material))
public:
  MeshRender();
  MeshRender(Asset<Mesh> mesh, Asset<Material> material);
  void render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame = false) const;
  const Asset<Material>& get_material() const;
  Asset<Material>& get_material();

};
