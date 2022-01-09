#include "motion_matching.h"
#include "../settings.h"

AnimationIndex solve_motion_matching_vp_tree(
  AnimationDataBasePtr dataBase,
  const AnimationGoal &goal,
  float tolerance_error)
{
  if (!dataBase)
    return AnimationIndex();
  
  for (const auto &tree : dataBase->vpTrees)
  {
    if (has_goal_tags(goal.tags, tree.tag))
    {
      auto [clip, frame] = tree.find_closect(goal.feature, tolerance_error);
      return AnimationIndex(dataBase, clip, frame);
    }
  }
  return AnimationIndex();
}

AnimationIndex solve_motion_matching_cover_tree(
  AnimationDataBasePtr dataBase,
  const AnimationGoal &goal,
  float tolerance_error)
{
  if (!dataBase)
    return AnimationIndex();
  
  for (const auto &tree : dataBase->coverTrees)
  {
    if (has_goal_tags(goal.tags, tree.tag))
    {
      auto [clip, frame] = tree.find_closect(goal.feature, tolerance_error);
      return AnimationIndex(dataBase, clip, frame);
    }
  }
  return AnimationIndex();
}