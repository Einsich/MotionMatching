#pragma once
#include <functional>
#include "3dmath.h"
#include "Serialization/settings_set.h"
#include "AnimationDatabase/animation_nodes.h"
struct Settings : SettingsSet
{
  static inline Settings *instance;
  static inline int testCount = 0;
  static inline int earlyTestMMCount = 0;
  static inline int MMCount = 0;
  #define FVAR DECL_FVAR
  #define IVAR DECL_IVAR
  #define BVAR DECL_BVAR
  #define LABEL(s)

  #define PARAMS()\
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