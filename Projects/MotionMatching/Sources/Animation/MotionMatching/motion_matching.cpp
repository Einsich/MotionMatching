#include "motion_matching.h"
#include "../settings.h"
#include <map>

static std::map<AnimationDataBasePtr, MotionMatchingSolverPtr> solvers[(int)MotionMatchingSolverType::Count];

MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, string first_anim, MotionMatchingSolverType solverType):
dataBase(dataBase), solver(nullptr), index(dataBase, 0, 0), skip_time(0), lod(0)
{
  if (!dataBase)
    return;
  for (uint i = 0; i < dataBase->clips.size(); i++)
  {
    if (dataBase->clips[i].name == first_anim)
    {
      index = AnimationLerpedIndex(dataBase, i, 0);
      break;
    }
  }
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
  const MotionMatchingOptimisationSettings &optimisationSettings)
{
  constexpr float lod_skip_time[4] = {1.f / 30, 1.f / 3, 3.f, 30.f};
  if (!solver)
    return;
  AnimationIndex saveIndex = index.current_index();
  index.update(dt);
  skip_time += dt;

  if (saveIndex != index.current_index())
  {
    
    AnimationIndex currentIndex = index.current_index();
    bool forceJump = (currentIndex.get_cadr_index() + 1 == (int)currentIndex.get_clip().duration);
    lod = optimisationSettings.lodOptimisation ? lod : 0;
    if (forceJump || skip_time >= lod_skip_time[lod])
    {
      skip_time = 0;
      bestScore = {0,0,0,0,0};
      AnimationIndex best_index = solver->solve_motion_matching(currentIndex, goal, bestScore, settings, optimisationSettings);
      
      bool can_jump = true;
      for (const AnimationIndex &index : index.get_indexes())
        can_jump &= AnimationIndex::can_jump(index, best_index);
      if (can_jump)
      {
        index.play_lerped(best_index);
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