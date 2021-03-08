#include "motion_matching.h"
#include <map>

static std::map<AnimationDataBasePtr, MotionMatchingSolverPtr> solvers[(int)MotionMatchingSolverType::Count];
constexpr int max_skip_cadr = 10;
MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, int first_anim, MotionMatchingSolverType solverType):
dataBase(dataBase), solver(nullptr), index(dataBase, first_anim, 0, first_anim, 0), skip_count(0)
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
AnimationLerpedIndex MotionMatching::get_index()
{
  return index;
}
void MotionMatching::update(float dt, const AnimationGoal &goal)
{
  if (!solver)
    return;
  index.t += dt * index.ticks_per_second();
  while (index.t > 1.f)
  {
    bool play_next_cadr = skip_count < max_skip_cadr && (!index.second.last_cadr() || index.second.get_clip().contains_tag(AnimationTag::Loopable));
    if (play_next_cadr)
    {
      index.t -= 1.f;
      index.first = index.second;
      index.second.increase_cadr();
      skip_count++;
    }
    else
    {
      index.t = 0.f;
      index.first = index.second;
      index.second = solver->find_best_index(index.second, goal);
      skip_count = 0;
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