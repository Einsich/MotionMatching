#pragma once
#include <map>
#include <string>
#include "animation_nodes.h"

struct MotionMatchingWeights
{
public:
  static inline std::map<std::string, int> featureMap;
  static inline MotionMatchingWeights *instance;
  static inline int testCount = 0;
  MotionMatchingWeights()
  {
    #define FEATURE(node) {#node, (int)AnimationFeaturesNode::node}, {#node "Speed",(int)AnimationFeaturesNode::Count + (int)AnimationFeaturesNode::node}
    featureMap = {
        FEATURE(Hips),
        FEATURE(LeftHand),
        FEATURE(RightHand),
        FEATURE(LeftToeBase),
        FEATURE(RightToeBase)};

  }
};
#undef NODE