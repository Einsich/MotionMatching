#include "motion_matching.h"
#include "../settings.h"

struct ArgMin
{
  float value;
  uint clip, frame;
  MatchingScores score;
};

static ArgMin mm_min2(const ArgMin &a, const ArgMin &b)
{
  return a.value < b.value ? a : b; 
}

AnimationIndex solve_motion_matching(
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  const AnimationGoal &goal,
  MatchingScores &best_score,
  const MotionMatchingSettings &mmsettings)
{
  if (!dataBase || !index())
    return AnimationIndex();
  uint curClip = index.get_clip_index();
  uint curCadr = index.get_cadr_index();
  
  ArgMin best = {INFINITY, curClip, curCadr, best_score};
  #pragma omp declare reduction(mm_min: ArgMin: omp_out=mm_min2(omp_out, omp_in))\
    initializer(omp_priv={INFINITY, 0, 0,{0,0,0,0,0}})
  #pragma omp parallel for reduction(mm_min:best)
  for (uint nextClip = 0; nextClip < dataBase->clips.size(); nextClip++)
  {
    const AnimationClip &clip = dataBase->clips[nextClip];

    if (!has_goal_tags(goal.tags, clip.tags))
      continue;
    for (uint nextCadr = 0, n = clip.duration; nextCadr < n; nextCadr++)
    {
      MatchingScores score = get_score(clip.features[nextCadr], goal.feature, mmsettings);
      
      float matching = score.full_score;
      ArgMin cur = {matching, nextClip, nextCadr, score};
      best = mm_min2(best, cur);
    }
  }
  best_score = best.score;
  return AnimationIndex(dataBase, best.clip, best.frame);
}


void get_motion_matching_statistic(
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  const AnimationGoal &goal,
  const MotionMatchingSettings &mmsettings)
{
  if (!dataBase || !index())
    return;
  for (uint nextClip = 0; nextClip < dataBase->clips.size(); nextClip++)
  {
    const AnimationClip &clip = dataBase->clips[nextClip];
    float tagFactor = has_goal_tags(goal.tags, clip.tags) ? 1 : 100000;
    for (uint nextCadr = 0, n = clip.duration; nextCadr < n; nextCadr++)
    {
      MatchingScores score = get_score(clip.features[nextCadr], goal.feature, mmsettings);
      
      dataBase->matchingScore[nextClip][nextCadr] = score.full_score * tagFactor;
    }
  }
}