#include "motion_matching_brute_force.h"
MotionMatchingBruteSolver::MotionMatchingBruteSolver(AnimationDataBasePtr dataBase):
dataBase(dataBase)
{
  if (!dataBase)
    return;
  matchingScore.resize(dataBase->clips.size());
  for (uint i = 0; i < matchingScore.size(); i++)
    matchingScore[i].resize(dataBase->clips[i].duration, 0);
}
AnimationIndex MotionMatchingBruteSolver::find_best_index(const AnimationIndex &index, const AnimationGoal &goal)
{
  if (!dataBase || !index())
    return AnimationIndex();
  const AnimationFeatures &feature = index.get_feature();
  int curClip = index.get_clip_index();
  int curCadr = index.get_cadr_index();
  float best = INFINITY;
  int bestClip = curClip;
  int bestCadr = curCadr;
  bool forceJump = index.get_clip().loopable ? false : curCadr + 2 >= (int)index.get_clip().duration;
  for(int nextClip = 0; nextClip < (int)dataBase->clips.size(); nextClip++)
  {
    const AnimationClip &clip = dataBase->clips[nextClip];
    if (forceJump && nextClip == curClip)
      continue;
    for (int nextCadr = 0, n = dataBase->clips[nextClip].duration-0; nextCadr < n; nextCadr++)
    {
      MatchingScores score = get_score(clip.features[nextCadr], feature, clip.get_frame_trajectory(nextCadr), goal);
      float matching = score.full_score;
      matchingScore[nextClip][nextCadr] = matching;
      if (matching < best)
      {
        best = matching;
        bestClip = nextClip;
        bestCadr = nextCadr;
        bestScore = score;
      }
    }
  }

  return AnimationIndex(dataBase, bestClip, bestCadr);
}
const vector<vector<float>> &MotionMatchingBruteSolver::get_matching_scores() const
{
  return matchingScore;
}
AnimationDataBasePtr MotionMatchingBruteSolver::get_data_base() const
{
  return dataBase;
}