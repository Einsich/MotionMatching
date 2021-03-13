#pragma once
#include "AnimationDatabase/animation_path_feature.h"

#define TAGS ADD_TAG(Stay), ADD_TAG(Crouch), ADD_TAG(Jump), ADD_TAG(Loopable), ADD_TAG(Die), ADD_TAG(Speak), ADD_TAG(BadTag)

#define ADD_TAG(TAG) TAG
enum class AnimationTag
{
  TAGS
};
#undef ADD_TAG
class AnimationGoal
{
public:
  AnimationPathFeature path;
  vector<AnimationTag> tags;
};

const std::string &get_tag_name(AnimationTag tag);