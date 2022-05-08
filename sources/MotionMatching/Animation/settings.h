#pragma once
#include <functional>
#include "3dmath.h"
#include "AnimationDatabase/animation_nodes.h"
#include "singleton.h"
struct Settings : ecs::Singleton
{
  int TotalMMCount = 0;
  int afterLodOptimization = 0;
  int afterTrajectoryToleranceTest = 0;

REFLECT(
    Settings,
  (float)(mouseSensitivity),
  (bool) (mouseInvertXaxis),
  (bool) (disableCameraRotation),
  (float)(lerpTime),
  (int)  (maxLerpIndex),
  (bool) (debugNodes),
  (bool) (debugTrajectory),
  (bool) (debugBones),
  (bool) (MatchingStatistic),
  (int) (testCount),
  (float) (testDensity))

};
struct ControllerSettings
{
  REFLECT(
    ControllerSettings,
    (float) (walkForwardSpeed),
    (float) (walkSidewaySpeed),
    (float) (walkBackwardSpeed),
    (float) (runForwardSpeed),
    (float) (runSidewaySpeed),
    (float) (runBackwardSpeed),
    (float) (strafeAngle),
    (float) (strafeStartSpeed),
    (float) (accelerationRate),
    (float) (decelerationRate),
    (float) (inertionTime),
    (float) (startSpeed),
    (float) (rotationRate),
    (float) (maxMoveErrorRadius),
    (float) (onPlaceMoveError),
    (float) (onPlaceRotationError)
  )
};

struct MotionMatchingSettings
{  
  #define NODE(node) (float) (node), (float) (node##Speed)

  REFLECT(
    MotionMatchingSettings,
    (float) (realism),
    (float) (poseMatchingWeight),
    (bool)  (velocityMatching),
    (float) (velocityMatchingWeight),
    (float) (goalPathMatchingWeight),
    (float) (goalVelocityWeight),
    (float) (goalAngularVelocityWeight),
    NODE(LeftHand),
    NODE(RightHand),
    NODE(LeftToeBase),
    NODE(RightToeBase)
  )
  #undef NODE
  std::reference_wrapper<float> nodeWeights[(int)AnimationFeaturesNode::Count] = {LeftHand, RightHand, LeftToeBase, RightToeBase};
  std::reference_wrapper<float> velocitiesWeights[(int)AnimationFeaturesNode::Count] = {LeftHandSpeed, RightHandSpeed, LeftToeBaseSpeed, RightToeBaseSpeed};
  void update_refs()
  {
    int i = 0;
    #define NODE(node) nodeWeights[i] = node; velocitiesWeights[i++] = node##Speed;
    NODES
    #undef NODE
  }
};

struct MotionMatchingOptimisationSettings
{  
  REFLECT(
    MotionMatchingOptimisationSettings,
    (bool)  (trajectoryErrorToleranceTest),
    (float) (pathErrorTolerance),
    (float) (vpTreeErrorTolerance),
    (bool)  (lodOptimisation),
    (vec3)  (lodDistances),
    (vec4)  (lodSkipSeconds)
  )

};
struct SettingsContainer : ecs::Singleton
{
  REFLECT(
    SettingsContainer,
    (std::vector<std::pair<std::string, ControllerSettings>>) (controllerSettings),
    (std::vector<std::pair<std::string, MotionMatchingSettings>>) (motionMatchingSettings),
    (std::vector<std::pair<std::string, MotionMatchingOptimisationSettings>>) (motionMatchingOptimisationSettings)
  )
  void after_loading()
  {
    if (controllerSettings.empty())
      controllerSettings.emplace_back("default", ControllerSettings());
    if (motionMatchingSettings.empty())
      motionMatchingSettings.emplace_back("default", MotionMatchingSettings());
    if (motionMatchingOptimisationSettings.empty())
      motionMatchingOptimisationSettings.emplace_back("default", MotionMatchingOptimisationSettings());
    for (auto &settings : motionMatchingSettings)
      settings.second.update_refs();
  }
};