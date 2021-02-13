#pragma once
#include "CommonCode/common.h"
#include "../animation_index.h"
#include "motion_matching_brute_force.h"
#include <vector>
enum class MotionMatchingSolverType
{
  BruteForce,
  Count
};
class MotionMatching
{
private:
  AnimationDataBasePtr dataBase;
  MotionMatchingSolverPtr solver;
  AnimationLerpedIndex index;

public:
  MotionMatching(AnimationDataBasePtr dataBase, int first_anim, MotionMatchingSolverType solverType);

  AnimationLerpedIndex get_index();
  void update(float dt, const AnimationGoal &goal);
  MotionMatchingSolverPtr get_solver() const;
};