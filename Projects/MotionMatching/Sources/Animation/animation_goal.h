#pragma once
#include <set>
#include "AnimationDatabase/animation_trajectory.h"

#define TAGS ADD_TAG(Stay), ADD_TAG(Crouch), ADD_TAG(Jump), ADD_TAG(Idle), ADD_TAG(Speak), ADD_TAG(BadTag)

#define ADD_TAG(TAG) TAG
enum class AnimationTag
{
  TAGS
};
#undef ADD_TAG
class AnimationGoal
{
public:
  AnimationTrajectory path;
  set<AnimationTag> tags;
};

const std::string &get_tag_name(AnimationTag tag);