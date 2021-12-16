#include "motion_matching.h"
#include "../settings.h"

AnimationIndex solve_motion_matching_vp_tree(
  AnimationDataBasePtr dataBase,
  const AnimationGoal &goal)
{
  if (!dataBase)
    return AnimationIndex();
  
  for (const auto &tree : dataBase->vpTrees)
  {
    if (has_goal_tags(goal.tags, tree.tag))
    {
      auto [clip, frame] = tree.find_closect(goal.feature);
      return AnimationIndex(dataBase, clip, frame);
    }
  }
  return AnimationIndex();
}
