#pragma once
#include "CommonCode/common.h"
#include "CommonCode/Shader/shader.h"
#include "AnimationTree/animation_tree.h"
#include "CommonCode/Material/material.h"
#include "CommonCode/Mesh/vertex_array_object.h"
#include "CommonCode/Light/direction_light.h"
#include "CommonCode/Camera/camera.h"
#include <vector>
class BoneRender
{
private:
  Shader boneShader;
  MaterialPtr boneMaterial;
  VertexArrayObject arrow;
  vector<mat4> boneTransforms;
  vector<vec3> boneOffsets;
  void calculate_transforms(const mat4& transform, const AnimationTree &tree);
public:
  BoneRender();
  void render(const mat4& transform, const AnimationTree &tree, const Camera& mainCam, const DirectionLight& light);
};