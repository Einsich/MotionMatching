#include "motion_matching.h"
#include "../settings.h"
#include <map>
#include "application/profile_tracker.h"


AnimationIndex solve_motion_matching(
  bool updateScoreStatistic,
  AnimationDataBasePtr dataBase,
  const AnimationIndex &index,
  AnimationGoal &goal,
  MatchingScores &best_score,
  Settings &settings,
  const MotionMatchingSettings &mmsettings,
  const MotionMatchingOptimisationSettings &optimisationSettings);


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
    bool lastFrames = currentIndex.get_cadr_index() + 5 >= (int)currentIndex.get_clip().duration;
    bool forceJump = lastFrames && !currentIndex.get_clip().loopable;
    lod = optimisationSettings.lodOptimisation ? lod : 0;
    if (forceJump || skip_time >= optimisationSettings.lodSkipSeconds[lod])
    {
      skip_time = 0;
      bestScore = {0,0,0,0,0};
      AnimationIndex best_index;
      switch (solverType)
      {
      case MotionMatchingSolverType::BruteForce :
        best_index = solve_motion_matching(updateStatistic, dataBase, currentIndex, goal, bestScore, settings, mmsettings, optimisationSettings);
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
      else
      {
        if (forceJump && currentIndex.get_clip().nextClipIdx >= 0)
        {
          best_index.set_index(currentIndex.get_clip().nextClipIdx, 0);
          index.play_lerped(best_index, settings.maxLerpIndex);
        }
      }
    }
  }
}

AnimationDataBasePtr MotionMatching::get_data_base() const
{
  return dataBase;
}