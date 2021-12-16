#include "motion_matching.h"
#include "../settings.h"
#include <map>
#include "application/profile_tracker.h"


AnimationIndex solve_motion_matching(
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  const AnimationGoal &goal,
  MatchingScores &best_score,
  const MotionMatchingSettings &mmsettings);

void get_motion_matching_statistic(
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  const AnimationGoal &goal,
  const MotionMatchingSettings &mmsettings);

MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, AnimationLerpedIndex index, MotionMatchingSolverType solverType):
dataBase(dataBase), solverType(solverType), index(index), skip_time(0), lod(0)
{
  if (!dataBase)
    return;
  
  if (dataBase->matchingScore.empty())
  {
    dataBase->matchingScore.resize(dataBase->clips.size());
    for (uint i = 0; i < dataBase->matchingScore.size(); i++)
      dataBase->matchingScore[i].resize(dataBase->clips[i].duration, 0);
  }
}
AnimationLerpedIndex MotionMatching::get_index() const
{
  return index;
}

static bool trajection_tolerance_test(AnimationIndex index, const AnimationGoal &goal, float pathErrorTolerance, float rotationErrorTolerance)
{
  const AnimationClip &clip = index.get_clip();
  int frame = index.get_cadr_index();
  if (has_goal_tags(goal.tags, clip.tags))
  {
    const AnimationTrajectory &trajectory = clip.features[frame].trajectory;
    float trajectory_cost = goal_path_norma(trajectory, goal.feature.trajectory);
    float rotation_cost = rotation_norma(trajectory, goal.feature.trajectory);
    return trajectory_cost < pathErrorTolerance && rotation_cost < rotationErrorTolerance;
  }
  return false;
}

void MotionMatching::update(float dt, AnimationGoal &goal,
  const MotionMatchingSettings &mmsettings,
  const MotionMatchingOptimisationSettings &optimisationSettings,
  bool updateStatistic,
  Settings &settings)
{
  //PROFILE_TRACK(project_path("profile/brute_forse/perf.csv"), 2000);

  AnimationIndex saveIndex = index.current_index();
  index.update(dt, settings.lerpTime);
  skip_time += dt;

  AnimationIndex currentIndex = index.current_index();
  if (saveIndex != currentIndex)
  {
    settings.MMCount++;
    lod = optimisationSettings.lodOptimisation ? lod : 0;
    float lodSkipTime = optimisationSettings.lodSkipSeconds[lod];
    if (skip_time >= lodSkipTime)
    {
      skip_time -= lodSkipTime;
      if (optimisationSettings.trajectoryErrorToleranceTest &&
          trajection_tolerance_test(currentIndex, goal, optimisationSettings.pathErrorTolerance, optimisationSettings.rotationErrorTolerance))
      {
        settings.earlyTestMMCount++;
        return;
      }
      goal.feature.features = currentIndex.get_feature();
      if (updateStatistic)
      {
        get_motion_matching_statistic(dataBase, currentIndex, goal, mmsettings);
      }
      bestScore = {0,0,0,0,0};
      AnimationIndex best_index;
      switch (solverType)
      {
      case MotionMatchingSolverType::BruteForce :
        best_index = solve_motion_matching(dataBase, currentIndex, goal, bestScore, mmsettings);
        break;
      default: break;
      }
      
      bool can_jump = true;
      for (const AnimationIndex &index : index.get_indexes())
        can_jump &= AnimationIndex::can_jump(index, best_index);
      if (can_jump)
      {
        index.play_lerped(best_index, settings.maxLerpIndex);
      }
    }
  }
}

AnimationDataBasePtr MotionMatching::get_data_base() const
{
  return dataBase;
}