#pragma once
#include "../animation_index.h"

struct Settings;
struct MotionMatchingSettings;
struct MotionMatchingOptimisationSettings;
class MotionMatchingSolver
{
public:
  bool updateScoreStatistic;
  virtual AnimationIndex solve_motion_matching(const AnimationIndex &index,
      const AnimationGoal &goal,
      MatchingScores &best_score,
      Settings &settings,
      const MotionMatchingSettings &mmsettings,
      const MotionMatchingOptimisationSettings &optimisationSettings) = 0;
};
using MotionMatchingSolverPtr = shared_ptr<MotionMatchingSolver>;