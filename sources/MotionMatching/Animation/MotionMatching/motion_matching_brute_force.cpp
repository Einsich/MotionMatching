#include "motion_matching.h"
#include "../settings.h"

struct ArgMin
{
  float value;
  int clip, frame;
  MatchingScores score;
};

ArgMin mm_min2(const ArgMin &a, const ArgMin &b)
{
  return a.value < b.value ? a : b; 
}

AnimationIndex solve_motion_matching(
  bool updateScoreStatistic,
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  AnimationGoal &goal,
  MatchingScores &best_score,
  Settings &settings,
  const MotionMatchingSettings &mmsettings,
  const MotionMatchingOptimisationSettings &optimisationSettings)
{
  if (!dataBase || !index())
    return AnimationIndex();
  goal.feature.features = index.get_feature();
  int curClip = index.get_clip_index();
  int curCadr = index.get_cadr_index();

  const AnimationClip &clip = index.get_clip();
  bool forceJump = !clip.loopable && clip.nextClipIdx < 0 && curCadr + 2 >= (int)clip.duration;
  int nextCadr = (curCadr + 1) % (int)clip.duration;
  settings.MMCount++;
  if (!forceJump && optimisationSettings.trajectoryErrorToleranceTest &&
    has_goal_tags(goal.tags, clip.tags))
  {
    const AnimationTrajectory &trajectory = clip.features[nextCadr].trajectory;
    float trajectory_cost = goal_path_norma(trajectory, goal.feature.trajectory) ;
    float rotation_cost = rotation_norma(trajectory, goal.feature.trajectory);
    if (trajectory_cost < optimisationSettings.pathErrorTolerance &&
        rotation_cost < optimisationSettings.rotationErrorTolerance)
    {
      settings.earlyTestMMCount++;
      return AnimationIndex(dataBase, curClip, nextCadr);
    }
  }
  ArgMin best = {INFINITY, curClip, curCadr, best_score};
  #pragma omp declare reduction(mm_min: ArgMin: omp_out=mm_min2(omp_out, omp_in))\
    initializer(omp_priv={INFINITY, 0, 0,{0,0,0,0,0}})
  #pragma omp parallel for reduction(mm_min:best)
  for(int nextClip = 0; nextClip < (int)dataBase->clips.size(); nextClip++)
  {
    const AnimationClip &clip = dataBase->clips[nextClip];
    if (forceJump && nextClip == curClip)
      continue;
    if (!has_goal_tags(goal.tags, clip.tags))
      continue;
    for (int nextCadr = 0, n = clip.duration; nextCadr < n; nextCadr++)
    {
      MatchingScores score = get_score(clip.features[nextCadr], goal.feature, mmsettings);
      
      float matching = score.full_score;
      ArgMin cur = {matching, nextClip, nextCadr, score};
      if (updateScoreStatistic)
        dataBase->matchingScore[nextClip][nextCadr] = matching;
      best = mm_min2(best, cur);
    }
  }
  best_score = best.score;
  return AnimationIndex(dataBase, best.clip, best.frame);
}
