#pragma once
#include "common.h"
#include "Serialization/serialization.h"
#include <map>
#include <vector>
enum class AnimationFeaturesNode
{
  Hips,
  LeftHand,
  RightHand,
  LeftToeBase,
  RightToeBase,
  Count
};
class AnimationFeaturesWeights : public ISerializable
{
public:
  float norma_function_weight, goal_path_weight, goal_rotation, goal_tag_weight, next_cadr_weight, noise_scale, debug_scale, animation_lerp;
  vector<float> weights;
  AnimationFeaturesWeights();
  map<string, AnimationFeaturesNode> featureMap;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationFeaturesWeightsPtr = shared_ptr<AnimationFeaturesWeights>;