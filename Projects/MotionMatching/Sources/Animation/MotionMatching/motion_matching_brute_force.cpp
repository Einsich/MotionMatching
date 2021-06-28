#include "motion_matching_brute_force.h"
#include "../settings.h"
MotionMatchingBruteSolver::MotionMatchingBruteSolver(AnimationDataBasePtr dataBase):
dataBase(dataBase)
{
  if (!dataBase)
    return;
  matchingScore.resize(dataBase->clips.size());
  for (uint i = 0; i < matchingScore.size(); i++)
    matchingScore[i].resize(dataBase->clips[i].duration, 0);
}
struct ArgMin
{
  float value;
  int clip, frame;
  MatchingScores score;
};
ArgMin mm_min2(const ArgMin &a, const ArgMin &b) {
    return a.value < b.value ? a : b; 
}
AnimationIndex MotionMatchingBruteSolver::solve_motion_matching(
  const AnimationIndex &index,
  const AnimationGoal &goal,
  MatchingScores &best_score,
  const MotionMatchingSettings &settings,
  const MotionMatchingOptimisationSettings &optimisationSettings)
{
  if (!dataBase || !index())
    return AnimationIndex();
  const AnimationFeatures &feature = index.get_feature();
  int curClip = index.get_clip_index();
  int curCadr = index.get_cadr_index();

  const AnimationClip &clip = index.get_clip();
  bool forceJump = !clip.loopable && clip.nextClipIdx < 0 && curCadr + 2 >= (int)clip.duration;
  int nextCadr = (curCadr + 1) % (int)clip.duration;
  Settings::MMCount++;
  if (!forceJump && optimisationSettings.trajectoryErrorToleranceTest &&
    has_goal_tags(goal.tags, clip.tags))
  {
    const AnimationTrajectory &trajectory = clip.trajectories[nextCadr];
    float trajectory_cost = goal_path_norma(trajectory, goal) ;
    float rotation_cost = rotation_norma(trajectory, goal);
    if (trajectory_cost < optimisationSettings.pathErrorTolerance &&
        rotation_cost < optimisationSettings.rotationErrorTolerance)
    {
      Settings::earlyTestMMCount++;
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
    for (int nextCadr = 0, n = dataBase->clips[nextClip].duration; nextCadr < n; nextCadr++)
    {
      MatchingScores score = get_score(clip.features[nextCadr], clip.tags, feature, clip.trajectories[nextCadr], goal, settings);
      
      float matching = score.full_score;
      ArgMin cur = {matching, nextClip, nextCadr, score};
      if (updateScoreStatistic)
        matchingScore[nextClip][nextCadr] = matching;
      best = mm_min2(best, cur);
    }
  }
  best_score = best.score;
  return AnimationIndex(dataBase, best.clip, best.frame);
}
const vector<vector<float>> &MotionMatchingBruteSolver::get_matching_scores() const
{
  return matchingScore;
}
AnimationDataBasePtr MotionMatchingBruteSolver::get_data_base() const
{
  return dataBase;
}