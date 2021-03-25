#include "animation_index.h"

AnimationIndex::AnimationIndex(AnimationDataBasePtr dataBase, int clip, int cadr):
dataBase(dataBase), clip(-1), cadr(-1)
{
  set_index(clip, cadr);
}
bool AnimationIndex::valid() const
{
  return dataBase && 0 <= clip && clip < (int)dataBase->clips.size() && 0 <= cadr && cadr < (int)dataBase->clips[clip].duration;
}
void AnimationIndex::set_index(int clip, int cadr)
{
  if (dataBase)
  {
    if (0 <= clip && clip < (int)dataBase->clips.size() && 0 <= cadr && cadr < (int)dataBase->clips[clip].duration)
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
void AnimationIndex::increase_cadr()
{
  if (valid())
  {
    cadr++;
    if (cadr >= (int)dataBase->clips[clip].duration)
    {
      cadr = 0;
    }
  }
}
bool AnimationIndex::operator()() const
{
  return valid();
}
bool AnimationIndex::chack_data_base(AnimationDataBasePtr other_data_base) const
{
  return dataBase == other_data_base;
}
bool AnimationIndex::last_cadr() const
{
  return valid() && (int)dataBase->clips[clip].duration == cadr + 1;
}
int AnimationIndex::get_clip_index() const
{
  return clip;
}
int AnimationIndex::get_cadr_index() const
{
  return cadr;
}
const AnimationClip &AnimationIndex::get_clip() const
{
  return dataBase->clips[clip];
}
AnimationCadr AnimationIndex::get_cadr() const
{
  return dataBase->clips[clip].get_frame(cadr);
}
const AnimationFeatures &AnimationIndex::get_feature() const
{
  return dataBase->clips[clip].features[cadr];
}
AnimationTrajectory AnimationIndex::get_trajectory() const
{
  return dataBase->clips[clip].get_frame_trajectory(cadr);
}

AnimationLerpedIndex::AnimationLerpedIndex(
  AnimationDataBasePtr dataBase, int clip1, int cadr1, int clip2, int cadr2, float t):
dataBase(dataBase), first(dataBase, clip1, cadr1), second(dataBase, clip2, cadr2), t(t)
{}
AnimationCadr AnimationLerpedIndex::get_lerped_cadr()
{
  if (!second())
  {
    return first() ? first.get_cadr() : AnimationCadr();
  }
  return lerped_cadr(first.get_cadr(), second.get_cadr(), t);
}
AnimationDataBasePtr AnimationLerpedIndex::get_data_base() const
{
  return dataBase;
}
float AnimationLerpedIndex::ticks_per_second() const
{
  return dataBase ? (dataBase->clips.size() > 0 ? dataBase->clips[0].ticksPerSecond : 0) : 0;
}
void AnimationLerpedIndex::update(float dt)
{
  if (first() && second())
  {
    t += dt * ticks_per_second();
    if (t >= 1.f)
    {
      first = second;
      second.increase_cadr();
      t -= 1;
    }
  }
}