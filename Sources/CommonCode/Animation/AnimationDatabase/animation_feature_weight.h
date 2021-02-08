#pragma once
#include "CommonCode/common.h"
#include "CommonCode/Serialization/serialization.h"
#include <map>
#include <vector>
enum class AnimationFeaturesNode
{
  Hips,
  LeftForeArm,
  LeftHand,
  RightForeArm,
  RightHand,
  LeftLeg,
  LeftFoot,
  RightLeg,
  RightFoot,
  Head,
  Count
};
class AnimationFeaturesWeights : public ISerializable
{
public:
  float norma_function_weight;
  vector<float> weights;
  AnimationFeaturesWeights();
  map<string, AnimationFeaturesNode> featureMap;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationFeaturesWeightsPtr = shared_ptr<AnimationFeaturesWeights>;