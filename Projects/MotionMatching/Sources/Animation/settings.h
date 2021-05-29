#pragma once
#include <functional>
#include "3dmath.h"
#include "Serialization/settings_set.h"
#include "AnimationDatabase/animation_nodes.h"
struct Settings : SettingsSet
{
  static inline Settings *instance;
  #define FVAR DECL_FVAR
  #define IVAR DECL_IVAR
  #define BVAR DECL_BVAR
  #define LABEL(s)

  #define PARAMS()\
  FVAR(walkForwardSpeed, 1.2f, 0.f, 20.f)\
  FVAR(walkSidewaySpeed, 1.2f, 0.f, 20.f)\
  FVAR(walkBackwardSpeed, 1.0f, 0.f, 20.f)\
  FVAR(runForwardSpeed, 2.7f*1.2f, 0.f, 20.f)\
  FVAR(runSidewaySpeed, 2.f*1.2f, 0.f, 20.f)\
  FVAR(runBackwardSpeed, 2.1f*1.2f, 0.f, 20.f)\
  FVAR(hipsHeightStand, 0.967f, 0.f, 1.f)\
  FVAR(hipsHeightCrouch, 0.35f, 0.f, 1.f)\
  FVAR(hipsHeightCrouchIdle, 0.35f, 0.f, 1.f)\
  FVAR(hipsHeightJump, 1.2f, 0.f, 2.f)\
  FVAR(predictionMoveRate, 9.0f, 0.f, 20.f)\
  FVAR(predictionRotationRate, 9.0f, 0.f, 20.f)\
  FVAR(rotationRate, 2.0f, 0.f, 20.f)\
  FVAR(maxMoveErrorRadius, 1.0f, 0.f, 5.f)\
  FVAR(onPlaceMoveError, 0.0f, 0.f, 1.f)\
  FVAR(onPlaceRotationError, 0.0f, 0.f, 30.f)\
  FVAR(mouseSensitivity, 0.2f, 0.f, 1.f)\
  BVAR(mouseInvertXaxis, true)\
  BVAR(disableCameraRotation, true)\
  FVAR(lerpTime, 0.2f, 0.f, 1.f)\
  IVAR(maxLerpIndex, 2, 2, 10)\
  BVAR(debugNodes, false)\
  BVAR(debugTrajectory, false)\
  BVAR(debugBones, false)\
  BVAR(MatchingStatistic, false)

  PARAMS()
  #undef FVAR
  #undef IVAR
  #undef BVAR
  #undef LABEL


  Settings()
  {
    #define FVAR INIT_FVAR
    #define IVAR INIT_IVAR
    #define BVAR INIT_BVAR
    #define LABEL INIT_LABEL
    PARAMS() 
    #undef PARAMS
    #undef FVAR
    #undef IVAR
    #undef BVAR
    #undef LABEL
  }


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
    (float) (hipsHeightStand),
    (float) (hipsHeightCrouch),
    (float) (hipsHeightCrouchIdle),
    (float) (hipsHeightJump),
    (float) (predictionMoveRate),
    (float) (predictionRotationRate),
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
    (float) (goalRotationMatchingWeight),
    (float) (goalFavourTagWeight),
    NODE(Hips),
    NODE(LeftHand),
    NODE(RightHand),
    NODE(LeftToeBase),
    NODE(RightToeBase)
  )
  #undef NODE
  std::reference_wrapper<float> nodeWeights[(int)AnimationFeaturesNode::Count] = {Hips, LeftHand, RightHand, LeftToeBase, RightToeBase};
  std::reference_wrapper<float> velocitiesWeights[(int)AnimationFeaturesNode::Count] = {HipsSpeed, LeftHandSpeed, RightHandSpeed, LeftToeBaseSpeed, RightToeBaseSpeed};
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
    (float) (rotationErrorTolerance),
    (bool)  (lodOptimisation),
    (vec3)  (lodDistances),
    (vec4)  (lodSkipSeconds)
  )

};
struct SettingsContainer
{
  static inline SettingsContainer *instance;
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