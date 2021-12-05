#pragma once
#include "AnimationDatabase/animation_trajectory.h"
#include "animation_tag.h"

class AnimationGoal
{
public:
  bool rotatable;
  AnimationTrajectory path; 
  AnimationTags tags;
};