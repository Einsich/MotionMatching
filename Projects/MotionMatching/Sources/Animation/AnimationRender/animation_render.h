#pragma once
#include "Animation/AnimationTree/animation_tree.h"
#include "Mesh/mesh.h"
#include "Shader/shader.h"
#include "Material/material.h"
#include "Light/direction_light.h"
#include "Transform/transform.h"
class AnimationRender
{
private:
  MeshPtr mesh;
  MaterialPtr material;
  Shader shader;
public:
  AnimationRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader);
  void render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position,  const DirectionLight& light, const AnimationTree &tree, bool wire_frame = false) const ;
  MaterialPtr get_material() const;
};
