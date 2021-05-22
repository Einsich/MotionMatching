#pragma once
#include "common.h"
#include "Serialization/settings_set.h"
#include <functional>

#define NODES\
  NODE(Hips)\
  NODE(LeftHand)\
  NODE(RightHand)\
  NODE(LeftToeBase)\
  NODE(RightToeBase)
enum class AnimationFeaturesNode
{
#define NODE(node) node,
  NODES
  Count
#undef NODE
};
struct MotionMatchingWeights : public SettingsSet
{
public:
  static inline map<string, int> featureMap;
  static inline MotionMatchingWeights *instance;
  #define FVAR DECL_FVAR
  #define IVAR DECL_IVAR
  #define BVAR DECL_BVAR
  #define LABEL(s)

#define NODE(node) FVAR(node, 0.5f, 0.f, 1.f) FVAR(node##Speed, 0.5f, 0.f, 1.f)\

  #define PARAMS()\
  FVAR(poseMatchingWeight, 1.2f, 0.f, 50.f)\
  FVAR(velocityMatchingWeight, 1.2f, 0.f, 50.f)\
  FVAR(goalPathMatchingWeight, 1.2f, 0.f, 50.f)\
  FVAR(goalRotationMatchingWeight, 1.0f, 0.f, 50.f)\
  FVAR(goalFavourTagWeight, 0.f, 0.f, 20.f)\
  FVAR(YpathScale, 1.2f, 0.f, 5.f)\
  BVAR(trajectoryErrorToleranceTest, true)\
  FVAR(trajectoryErrorTolerance, 0.f, 0.f, 5.f)\
  BVAR(velocityMatching, true)\
  NODES

  PARAMS()
  #undef FVAR
  #undef IVAR
  #undef BVAR
  #undef LABEL
  static inline std::reference_wrapper<float> nodeWeights[(int)AnimationFeaturesNode::Count] = {Hips, LeftHand, RightHand, LeftToeBase, RightToeBase};
  static inline std::reference_wrapper<float> velocitiesWeights[(int)AnimationFeaturesNode::Count] = {HipsSpeed, LeftHandSpeed, RightHandSpeed, LeftToeBaseSpeed, RightToeBaseSpeed};

  MotionMatchingWeights()
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
    #define FEATURE(node) {#node, (int)AnimationFeaturesNode::node}, {#node "Speed",(int)AnimationFeaturesNode::Count + (int)AnimationFeaturesNode::node}
    featureMap = {
        FEATURE(Hips),
        FEATURE(LeftHand),
        FEATURE(RightHand),
        FEATURE(LeftToeBase),
        FEATURE(RightToeBase)};

  }
};