#pragma once
#include "CommonCode/component.h"
#include "CommonCode/Animation/AnimationTree/animation_tree.h"
#include "bone_render.h"
#include "CommonCode/Mesh/mesh.h"
#include "CommonCode/Shader/shader.h"
#include "CommonCode/Material/material.h"
#include "CommonCode/Camera/camera.h"
#include "CommonCode/Light/direction_light.h"
#include "CommonCode/Transform/transform.h"
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
