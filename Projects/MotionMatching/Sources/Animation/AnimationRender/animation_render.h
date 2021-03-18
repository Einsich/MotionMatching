#pragma once
#include "component.h"
#include "Animation/AnimationTree/animation_tree.h"
#include "bone_render.h"
#include "Mesh/mesh.h"
#include "Shader/shader.h"
#include "Material/material.h"
#include "Camera/camera.h"
#include "Light/direction_light.h"
#include "Transform/transform.h"
class AnimationRender : public Component, public IRenderable
{
private:
  MeshPtr mesh;
  MaterialPtr material;
  Shader shader;
  BoneRender boneRender;
  bool renderBones;
public:
  AnimationRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader, bool renderBones);
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;
  void render(const Transform &transform, const Camera& mainCam, const DirectionLight& light, const AnimationTree &tree, bool wire_frame = false);
  MaterialPtr get_material() const;
};
