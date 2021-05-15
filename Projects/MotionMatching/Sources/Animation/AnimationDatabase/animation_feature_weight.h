#pragma once
#include "common.h"
#include "Serialization/serialization.h"
#include <map>
#include <vector>

#define NODE(node) node
enum class AnimationFeaturesNode
{
  NODE(Hips),
  NODE(LeftHand),
  NODE(RightHand),
  NODE(LeftToeBase),
  NODE(RightToeBase),
  Count
};
class AnimationFeaturesWeights final : public ISerializable
{
public:
  float norma_function_weight, goal_path_weight, goal_rotation, goal_tag_weight, y_norma_scale, noise_scale, debug_scale, animation_lerp;
  vector<float> weights;
  AnimationFeaturesWeights();
  ~AnimationFeaturesWeights()=default;
  map<string, int> featureMap;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationFeaturesWeightsPtr = shared_ptr<AnimationFeaturesWeights>;