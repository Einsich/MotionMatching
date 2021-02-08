#pragma once
#include "AnimationDatabase/animation_path_feature.h"
enum class AnimationTag
{
  Stay, Crouch, Jump
};
class AnimationGoal
{
public:
  AnimationPathFeature path;
  vector<AnimationTag> tags;
};
