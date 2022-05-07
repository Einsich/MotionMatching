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


AnimationIndex solve_motion_matching_vp_tree(
  AnimationDataBasePtr dataBase,
  const AnimationGoal &goal,
  float tolerance_erorr);
  
AnimationIndex solve_motion_matching_cover_tree(
  AnimationDataBasePtr dataBase,
  const AnimationGoal &goal,
  float tolerance_erorr);

MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, AnimationLerpedIndex index):
dataBase(dataBase), index(index), skip_time(0), lod(0)
{  
}
AnimationLerpedIndex MotionMatching::get_index() const
{
  return index;
}

static bool trajection_tolerance_test(AnimationIndex index, const AnimationGoal &goal, const MotionMatchingSettings &mmsettings, float pathErrorTolerance)
{
  const AnimationClip &clip = index.get_clip();
  int frame = index.get_cadr_index();
  if (has_goal_tags(goal.tags, clip.tags))
  {
    const AnimationTrajectory &trajectory = clip.features[frame].trajectory;
    float path_cost = path_norma(trajectory, goal.feature.trajectory, mmsettings);
    return path_cost < pathErrorTolerance;
  }
  return false;
}

void MotionMatching::update(float dt, MotionMatchingSolverType solver_type, AnimationGoal &goal,
  const MotionMatchingSettings &mmsettings,
  const MotionMatchingOptimisationSettings &optimisationSettings,
  bool updateStatistic,
  Settings &settings)
{
  //PROFILE_TRACK(project_path("profile/brute_forse/perf.csv"), 5000);

  AnimationIndex saveIndex = index.current_index();
  index.update(dt, settings.lerpTime);
  skip_time += dt;
  //static vector<ProfileTracker> trackers(4, ProfileTracker(project_path("profile/vp_tree/perf.csv"), 1500));
  //ProfileTrack track(trackers[goal.tags.tags]);

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
          trajection_tolerance_test(currentIndex, goal, mmsettings, optimisationSettings.pathErrorTolerance))
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
      switch (solver_type)
      {
      default: 
      case MotionMatchingSolverType::BruteForce :
        best_index = solve_motion_matching(dataBase, currentIndex, goal, bestScore, mmsettings);
        break;
      case MotionMatchingSolverType::VPTree :
        best_index = solve_motion_matching_vp_tree(dataBase, goal, optimisationSettings.vpTreeErrorTolerance);
        break;
      case MotionMatchingSolverType::CoverTree :
        best_index = solve_motion_matching_cover_tree(dataBase, goal, optimisationSettings.vpTreeErrorTolerance);
        break;
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