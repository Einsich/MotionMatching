#pragma once
#include "common.h"
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
  float skip_time;
public:
  int lod;
  MatchingScores bestScore;
  MotionMatching(AnimationDataBasePtr dataBase, string first_anim, MotionMatchingSolverType solverType);

  AnimationLerpedIndex get_index() const;
  void update(float dt, const AnimationGoal &goal,
    const MotionMatchingSettings &settings,
    const MotionMatchingOptimisationSettings &optimisationSettings,
    bool updateStatistic);
  AnimationDataBasePtr get_data_base() const;
  MotionMatchingSolverPtr get_solver() const;
};