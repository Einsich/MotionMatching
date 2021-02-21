#pragma once
#include "CommonCode/common.h"
#include "CommonCode/Animation/AnimationTree/animation_tree.h"
#include <vector>
class BoneRender
{
private:
  vector<vec3> boneOffsets;
  void calculate_transforms(const mat4& transform, const AnimationTree &tree);
public:
  BoneRender();
  void render(const mat4& transform, const AnimationTree &tree);
};