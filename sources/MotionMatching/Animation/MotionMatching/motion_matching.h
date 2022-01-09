#pragma once
#include "common.h"
#include "../animation_index.h"
#include <vector>

enum class MotionMatchingSolverType
{
  BruteForce,
  VPTree,
  CoverTree,
};

struct Settings;
struct MotionMatchingSettings;
struct MotionMatchingOptimisationSettings;

class MotionMatching
{
public:
  AnimationDataBasePtr dataBase;
  AnimationLerpedIndex index;
  float skip_time;
  int lod;
  MatchingScores bestScore;
  MotionMatching(AnimationDataBasePtr dataBase, AnimationLerpedIndex index);
  MotionMatching() = default;
  AnimationLerpedIndex get_index() const;
  void update(float dt, MotionMatchingSolverType solver_type, AnimationGoal &goal,
    const MotionMatchingSettings &mmsettings,
    const MotionMatchingOptimisationSettings &optimisationSettings,
    bool updateStatistic,
    Settings &settings);
  AnimationDataBasePtr get_data_base() const;
};