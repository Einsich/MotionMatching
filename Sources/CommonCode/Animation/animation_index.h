#pragma once
#include "AnimationDatabase/animation_database.h"
class AnimationIndex
{
private:
  AnimationDataBasePtr dataBase;
  int clip, cadr;
public:
  AnimationIndex(AnimationDataBasePtr dataBase = nullptr, int clip = -1, int cadr = -1):
  dataBase(dataBase), clip(-1), cadr(-1)
  {
    set_index(clip, cadr);
  }
  void set_index(int clip, int cadr)
  {
    if (dataBase)
    {
      if (0 <= clip && clip < (int)dataBase->clips.size() && 0 <= cadr && cadr < (int)dataBase->clips[clip].cadres.size())
      {
        this->clip = clip;
        this->cadr = cadr;
      }
      else
      {
        dataBase = nullptr;
      }      
    }
  }
  bool operator()() const
  {
    return dataBase != nullptr;
  }
  bool chack_data_base(AnimationDataBasePtr other_data_base) const
  {
    return dataBase == other_data_base;
  }
  int get_clip_index() const
  {
    return clip;
  }
  int get_cadr_index() const
  {
    return cadr;
  }
  const AnimationClip &get_clip() const
  {
    return dataBase->clips[clip];
  }
  const AnimationCadr &get_cadr() const
  {
    return dataBase->clips[clip].cadres[cadr];
  }
  const AnimationFeatures &get_feature() const
  {
    return dataBase->clips[clip].features[cadr];
  }
};
class AnimationLerpedIndex
{
private:
  AnimationDataBasePtr dataBase;
public:
  AnimationIndex first, second;
  float t;
  AnimationLerpedIndex(AnimationDataBasePtr dataBase = nullptr, int clip1 = -1, int cadr1 = -1, int clip2 = -1, int cadr2 = -1, float t = 0):
  dataBase(dataBase), first(dataBase, clip1, cadr1), second(dataBase, clip2, cadr2), t(t)
  {}
  AnimationCadr get_lerped_cadr()
  {
    if (!second())
    {
      return first() ? first.get_cadr() : AnimationCadr();
    }
    return lerped_cadr(first.get_cadr(), second.get_cadr(), t);
  }
  float ticks_per_second() const
  {
    return dataBase ? (dataBase->clips.size() > 0 ? dataBase->clips[0].ticksPerSecond : 0) : 0;
  }
};