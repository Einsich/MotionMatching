#include "motion_matching.h"
#include "../settings.h"
#include <map>

static std::map<AnimationDataBasePtr, MotionMatchingSolverPtr> solvers[(int)MotionMatchingSolverType::Count];

MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, AnimationLerpedIndex index, MotionMatchingSolverType solverType):
dataBase(dataBase), solver(nullptr), index(index), skip_time(0), lod(0)
{
  if (!dataBase)
    return;
  auto &solverMap = solvers[(int)solverType];
  auto it = solverMap.find(dataBase);
  if (it == solverMap.end())
  {
    switch (solverType)
    {
    case MotionMatchingSolverType::BruteForce: solver = make_shared<MotionMatchingBruteSolver>(dataBase); break;
    default:solver = nullptr; break;
    }
    solverMap.emplace(dataBase, solver);
  }
  else
  {
    solver = it->second;
  }
}
AnimationLerpedIndex MotionMatching::get_index() const
{
  return index;
}
void MotionMatching::update(float dt, const AnimationGoal &goal,
  const MotionMatchingSettings &settings,
  const MotionMatchingOptimisationSettings &optimisationSettings,
  bool updateStatistic)
{
  
  if (!solver)
    return;
  AnimationIndex saveIndex = index.current_index();
  index.update(dt);
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
      solver->updateScoreStatistic = updateStatistic;
      AnimationIndex best_index = solver->solve_motion_matching(currentIndex, goal, bestScore, settings, optimisationSettings);
      
      bool can_jump = true;
      for (const AnimationIndex &index : index.get_indexes())
        can_jump &= AnimationIndex::can_jump(index, best_index);
      if (can_jump)
      {
        index.play_lerped(best_index);
      }
      else
      {
        if (forceJump && currentIndex.get_clip().nextClipIdx >= 0)
        {
          best_index.set_index(currentIndex.get_clip().nextClipIdx, 0);
          index.play_lerped(best_index);
        }
      }
    }
  }
}

MotionMatchingSolverPtr MotionMatching::get_solver() const
{
  return solver;
}
AnimationDataBasePtr MotionMatching::get_data_base() const
{
  return dataBase;
}