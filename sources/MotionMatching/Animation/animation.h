#pragma once
#include "AnimationDatabase/animation_cadr.h"
#include "AnimationDatabase/animation_clip.h"

class Animation
{
public:
  Animation(const AnimationClip &clip):clip(clip), cadr(0), t(0){}
  const AnimationClip &clip;
  int cadr; 
  float t;
  AnimationCadr get_lerped_cadr() const;
  float ticksPerSecond() const;
  void update(float dt);
  bool ended();
};
