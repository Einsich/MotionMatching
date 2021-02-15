#pragma once
#include "AnimationDatabase/animation_database.h"
class AnimationIndex
{
private:
  AnimationDataBasePtr dataBase;
  int clip, cadr;
public:
  AnimationIndex(AnimationDataBasePtr dataBase = nullptr, int clip = -1, int cadr = -1);
  bool valid() const;
  void set_index(int clip, int cadr);
  void increase_cadr();
  bool operator()() const;
  bool chack_data_base(AnimationDataBasePtr other_data_base) const;
  int get_clip_index() const;
  int get_cadr_index() const;
  const AnimationClip &get_clip() const;
  const AnimationCadr &get_cadr() const;
  const AnimationFeatures &get_feature() const;
};
class AnimationLerpedIndex
{
private:
  AnimationDataBasePtr dataBase;
public:
  AnimationIndex first, second;
  float t;
  AnimationLerpedIndex(AnimationDataBasePtr dataBase = nullptr, int clip1 = -1, int cadr1 = -1, int clip2 = -1, int cadr2 = -1, float t = 0);
  AnimationCadr get_lerped_cadr();
  AnimationDataBasePtr get_data_base() const;
  float ticks_per_second() const;
  void update(float dt);
};