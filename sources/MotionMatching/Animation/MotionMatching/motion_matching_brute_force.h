#pragma once
#include "common.h"
#include "motion_matching_solver.h"
#include <vector>

class MotionMatchingBruteSolver final : public MotionMatchingSolver
{

private:
  AnimationDataBasePtr dataBase;
  vector<vector<float>> matchingScore;
public:
  MotionMatchingBruteSolver(AnimationDataBasePtr dataBase);
  virtual AnimationIndex solve_motion_matching(const AnimationIndex &index,
      const AnimationGoal &goal,
      MatchingScores &best_score,
      Settings &settings,
      const MotionMatchingSettings &mmsettings,
      const MotionMatchingOptimisationSettings &optimisationSettings) override final;
  const vector<vector<float>> &get_matching_scores() const;
  AnimationDataBasePtr get_data_base() const;

};
using MotionMatchingBruteSolverPtr = shared_ptr<MotionMatchingBruteSolver>;