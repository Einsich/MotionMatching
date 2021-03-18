#pragma once
#include "common.h"
#include "motion_matching_solver.h"
#include <vector>

class MotionMatchingBruteSolver : public MotionMatchingSolver
{

private:
  AnimationDataBasePtr dataBase;
  vector<vector<float>> matchingScore;

public:
  MatchingScores bestScore;
  MotionMatchingBruteSolver(AnimationDataBasePtr dataBase);
  virtual AnimationIndex find_best_index(const AnimationIndex &index, const AnimationGoal &goal) override final;
  const vector<vector<float>> &get_matching_scores() const;
  AnimationDataBasePtr get_data_base() const;

};
using MotionMatchingBruteSolverPtr = shared_ptr<MotionMatchingBruteSolver>;