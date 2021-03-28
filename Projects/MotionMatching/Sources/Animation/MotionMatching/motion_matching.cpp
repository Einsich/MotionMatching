#include "motion_matching.h"
#include <map>

static std::map<AnimationDataBasePtr, MotionMatchingSolverPtr> solvers[(int)MotionMatchingSolverType::Count];
constexpr int max_skip_cadr = 10;
MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, string first_anim, MotionMatchingSolverType solverType):
dataBase(dataBase), solver(nullptr), index(dataBase, 0, 0, 0, 0), skip_count(0)
{
  if (!dataBase)
    return;
  for (int i = 0; i < dataBase->clips.size(); i++)
  {
    if (dataBase->clips[i].name == first_anim)
    {
      index = AnimationLerpedIndex(dataBase, i, 0, i, 0);
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
void MotionMatching::update(float dt, const AnimationGoal &goal)
{
  if (!solver)
    return;
  index.t += dt * index.ticks_per_second();
  if (index.t > 1.f)
  {
    AnimationIndex best_index = solver->find_best_index(index.second, goal);
    if (AnimationIndex::can_jump(index.second, best_index))
    {
      index.first = index.second;
      index.second = best_index;

    }
    else
    {
      index.first = index.second;
      index.second.increase_cadr();
    }
    index.t -= (int)index.t;
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