#include "animation_index.h"
#include "Animation/settings.h"

AnimationIndex::AnimationIndex(AnimationDataBasePtr dataBase, int clip, int cadr):
dataBase(dataBase), clip(-1), cadr(-1)
{
  set_index(clip, cadr);
}
bool AnimationIndex::valid() const
{
  return dataBase && 0 <= clip && clip < (int)dataBase->clips.size() && 0 <= cadr && cadr < (int)dataBase->clips[clip].duration;
}
AnimationIndex::operator bool() const
{
  return valid();
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
      //  --cadr;      
    }
  }
}
bool AnimationIndex::can_jump(const AnimationIndex &from, const AnimationIndex &to)
{
  constexpr int N = 15;
  #define ABS_L(k, n) (((k + n) % n) >= N)
  if (to.get_cadr_index() + 4 >= (int)to.get_clip().duration)
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
const AnimationTrajectory &AnimationIndex::get_trajectory() const
{
  return dataBase->clips[clip].trajectories[cadr];
}
bool AnimationIndex::operator==(const AnimationIndex &i2) const
{
  return cadr == i2.cadr && clip == i2.clip;
}
bool AnimationIndex::operator!=(const AnimationIndex &i2) const
{
  return !(*this == i2);
}

AnimationDataBasePtr AnimationIndex::get_data_base() const
{
  return dataBase;
}


AnimationLerpedIndex::AnimationLerpedIndex(
  AnimationDataBasePtr dataBase, int clip1, int cadr1):
dataBase(dataBase), indexes({AnimationIndex(dataBase, clip1, cadr1)}), indexesT({0})
{}
AnimationLerpedIndex::operator bool() const
{
  return indexes.size() > 0 && indexes.back();
}
AnimationCadr AnimationLerpedIndex::get_lerped_cadr()
{
  AnimationCadr lateFrame = indexes[0].get_cadr(indexesT[0]);
  for (int i = 1; i < (int)indexes.size(); i++)
  {
    lateFrame = lerped_cadr(lateFrame, indexes[i].get_cadr(indexesT[i]), mixWeights[i-1]);
  }
  return lateFrame;
}
AnimationDataBasePtr AnimationLerpedIndex::get_data_base() const
{
  return dataBase;
}
float AnimationLerpedIndex::ticks_per_second() const
{
  return dataBase ? (dataBase->clips.size() > 0 ? dataBase->clips[0].ticksPerSecond : 0) : 0;
}
void AnimationLerpedIndex::update(float dt, float lerpTime)
{

  for (int i = 0; i < (int)indexes.size() - 1; i++)
    mixWeights[i] += dt / lerpTime;
  dt *= ticks_per_second();
  for (int i = 0; i < (int)indexes.size(); i++)
    indexesT[i] += dt;


  while (indexes.size() > 1 && mixWeights[0] >= 1.f)
  {
    mixWeights.erase(mixWeights.begin());
    indexesT.erase(indexesT.begin());
    indexes.erase(indexes.begin());
  }

  for (int i = 0; i < (int)indexes.size(); i++)
  {
    if (indexesT[i] >= 1.f)
    {
      indexes[i].increase_cadr();
      indexesT[i] -= (int)indexesT[i];
    }
  }
}

void AnimationLerpedIndex::play_lerped(AnimationIndex next, int maxLerpIndex)
{
  maxLerpIndex = maxLerpIndex > 1 ? maxLerpIndex : 1;
  if ((int)indexes.size() > maxLerpIndex)
  {
    mixWeights.erase(mixWeights.begin());
    indexesT.erase(indexesT.begin());
    indexes.erase(indexes.begin());
  }
  indexes.push_back(next);
  indexesT.push_back(0);
  mixWeights.push_back(0);
}
AnimationIndex AnimationLerpedIndex::current_index() const
{
  return indexes.back();
}
const vector<AnimationIndex> &AnimationLerpedIndex::get_indexes() const
{
  return indexes;
}