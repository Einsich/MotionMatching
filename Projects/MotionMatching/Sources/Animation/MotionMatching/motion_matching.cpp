#include "motion_matching.h"
#include <map>

static std::map<AnimationDataBasePtr, MotionMatchingSolverPtr> solvers[(int)MotionMatchingSolverType::Count];
constexpr int max_skip_cadr = 10;
MotionMatching::MotionMatching(AnimationDataBasePtr dataBase, string first_anim, MotionMatchingSolverType solverType):
dataBase(dataBase), solver(nullptr), index(dataBase, 0, 0), skip_count(0)
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
void MotionMatching::update(float dt, const AnimationGoal &goal)
{
  if (!solver)
    return;
  float saveT = index.t;
  index.update(dt);

  if (saveT > index.t)
  {
    AnimationIndex currentIndex = index.current_index();
    AnimationIndex best_index = solver->find_best_index(currentIndex, goal);
    // debug_log("i = (%s, %d/%d), s = %f", best_index.get_clip().name.c_str(), best_index.get_cadr_index(), best_index.get_clip().duration, ((MotionMatchingBruteSolver*)solver.get())->bestScore.full_score);
    if (AnimationIndex::can_jump(currentIndex, best_index))
    {
      index.play_lerped(best_index, 0.2f);
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