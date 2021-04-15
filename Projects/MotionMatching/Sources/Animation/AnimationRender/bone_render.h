#pragma once
#include "common.h"
#include "Animation/AnimationTree/animation_tree.h"
#include <vector>
class BoneRender
{
public:
  BoneRender();
  void render(const mat4& transform, const AnimationTree &tree) const;
};