#pragma once
#include "../animation_index.h"

class MotionMatchingSolver
{
public:
  virtual AnimationIndex find_best_index(const AnimationIndex &index, const AnimationGoal &goal) = 0;
};
using MotionMatchingSolverPtr = shared_ptr<MotionMatchingSolver>;