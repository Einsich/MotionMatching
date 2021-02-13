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
  float best = 0;
  int bestClip = -1;
  int bestCadr = -1;
  const AnimationFeatures &feature = index.get_feature();
  int curClip = index.get_clip_index();
  int curCadr = index.get_cadr_index();
  for(uint nextClip = 0; nextClip < dataBase->clips.size(); nextClip++)
  {
    for (uint nextCadr = 0, n = dataBase->clips[nextClip].features.size(); nextCadr < n; nextCadr++)
    {
      const AnimationClip &clip = dataBase->clips[nextClip];
      float pose_match = pose_matching_norma(clip.features[nextCadr], feature);
      float goal_match = goal_matching_norma(clip.features[nextCadr].path, clip.tags, goal);
      float next_cadr_match = next_cadr_norma(curClip, curCadr, nextClip, nextCadr, clip.cadres.size());
      float matching = pose_match + goal_match + next_cadr_match;
      matchingScore[nextClip][nextCadr] = goal_match;
      if (matching > best)
      {
        best = matching;
        bestClip = nextClip;
        bestCadr = nextCadr;
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