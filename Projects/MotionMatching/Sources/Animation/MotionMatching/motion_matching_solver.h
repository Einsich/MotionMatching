#pragma once
#include "../animation_index.h"

class MotionMatchingSolver
{
public:
  virtual AnimationIndex solve_motion_matching(const AnimationIndex &index, const AnimationGoal &goal, MatchingScores &best_score) = 0;
};
using MotionMatchingSolverPtr = shared_ptr<MotionMatchingSolver>;