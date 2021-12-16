#pragma once
#include "common.h"
#include "../animation_index.h"
#include <vector>

enum class MotionMatchingSolverType
{
  BruteForce,
  VP_Tree
};

struct Settings;
struct MotionMatchingSettings;
struct MotionMatchingOptimisationSettings;

class MotionMatching
{
public:
  AnimationDataBasePtr dataBase;
  MotionMatchingSolverType solverType;
  AnimationLerpedIndex index;
  float skip_time;
  int lod;
  MatchingScores bestScore;
  MotionMatching(AnimationDataBasePtr dataBase, AnimationLerpedIndex index, MotionMatchingSolverType solverType);
  MotionMatching() = default;
  AnimationLerpedIndex get_index() const;
  void update(float dt, AnimationGoal &goal,
    const MotionMatchingSettings &mmsettings,
    const MotionMatchingOptimisationSettings &optimisationSettings,
    bool updateStatistic,
    Settings &settings);
  AnimationDataBasePtr get_data_base() const;
};