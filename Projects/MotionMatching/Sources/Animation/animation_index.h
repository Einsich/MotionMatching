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
  bool last_cadr() const;
  int get_clip_index() const;
  int get_cadr_index() const;
  const AnimationClip &get_clip() const;
  AnimationCadr get_cadr() const;
  AnimationCadr get_cadr(float t) const;
  const AnimationFeatures &get_feature() const;
  const AnimationTrajectory &get_trajectory() const;
  static bool can_jump(const AnimationIndex &from, const AnimationIndex &to);
  bool operator==(const AnimationIndex &i2);
  bool operator!=(const AnimationIndex &i2);
  AnimationDataBasePtr get_data_base() const;
};
class AnimationLerpedIndex
{
private:
  AnimationDataBasePtr dataBase;
  vector<AnimationIndex> indexes;//1+n
  vector<float> indexesT;//1+n
  vector<float> mixWeights;//n
public:
  AnimationLerpedIndex(AnimationDataBasePtr dataBase = nullptr, int clip1 = -1, int cadr1 = -1);
  AnimationCadr get_lerped_cadr();
  AnimationDataBasePtr get_data_base() const;
  float ticks_per_second() const;
  void update(float dt);
  void play_lerped(AnimationIndex next);
  AnimationIndex current_index() const;
  const vector<AnimationIndex> &get_indexes() const;
};