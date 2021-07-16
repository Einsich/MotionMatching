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
  MeshPtr mesh;
  REFLECT(MeshRender,
  (Asset<Material>) (material))
  Shader shader;
public:
  MeshRender() = default;
  MeshRender(MeshPtr mesh_ptr, Asset<Material> materail_ptr, const Shader& shader);
  void render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame = false) const;
  Asset<Material> get_material() const;
  const Shader& get_shader() const;
  Shader& get_shader();
};
MeshRender create_plane(bool create_uv = false);
MeshRender create_cube(bool create_uv = false);
MeshRender create_sphere(int detailed = 1, bool smooth = false, bool create_uv = false);
MeshRender create_plane(Asset<Material> material, bool create_uv = false);
MeshRender create_cube(Asset<Material> material, bool create_uv = false);
MeshRender create_sphere(Asset<Material> material, int detailed = 1, bool smooth = false, bool create_uv = false);
MeshRender create_postfx();
