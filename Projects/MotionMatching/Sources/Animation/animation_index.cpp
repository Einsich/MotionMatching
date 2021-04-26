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
      //if (dataBase->clips[clip].loopable)
        cadr = 0;
      //else
      //  cadr--;
    }
  }
}
bool AnimationIndex::can_jump(const AnimationIndex &from, const AnimationIndex &to)
{
  constexpr int N = 10;
  #define ABS_L(k, n) (((k + n) % n) >= N)
  if (to.get_cadr_index() + 4 >= to.get_clip().duration)
    return false;
  if (from.clip == to.clip)
  {
    //return false;//???
    int k = from.cadr - to.cadr;
    const AnimationClip &clip = to.get_clip();
    if (clip.loopable)
      return ABS_L(k, clip.duration) && ABS_L(-k, clip.duration);
    return abs(k) >= N;
  }
  return true;
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
AnimationCadr AnimationIndex::get_cadr(float t) const
{
  int nextClip = (cadr + 1 + dataBase->clips[clip].duration) % dataBase->clips[clip].duration;
  return lerped_cadr(dataBase->clips[clip].get_frame(cadr), dataBase->clips[clip].get_frame(nextClip), t);
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
  AnimationDataBasePtr dataBase, int clip1, int cadr1):
dataBase(dataBase), first(dataBase, clip1, cadr1), second(), t(0), lerpT(0), lerpTime(0)
{}
AnimationCadr AnimationLerpedIndex::get_lerped_cadr()
{
  if (!second())
  {
    return first() ? first.get_cadr(t) : AnimationCadr();
  }
  return lerped_cadr(first.get_cadr(t), second.get_cadr(t), lerpT / lerpTime);
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
  t += dt * ticks_per_second();

  if (first() && !second())
  {
    if (t >= 1.f)
    {
      first.increase_cadr();
      t -= (int)t;
    }
  }
  else
  if (first() && second())
  {
    lerpT += dt;
    if (t >= 1.f)
    {
      first.increase_cadr();
      second.increase_cadr();
      t -= (int)t;
    }
    if (lerpT >= lerpTime)
    {
      first = second;
      second = AnimationIndex();
    }
  }
}

void AnimationLerpedIndex::play_lerped(AnimationIndex next, float lerp_time)
{
  if (second()) 
    first = second;
  second = next;
  lerpT = 0;
  lerpTime = lerp_time;
}
AnimationIndex AnimationLerpedIndex::current_index() const
{
  return second() ? second : first;
}