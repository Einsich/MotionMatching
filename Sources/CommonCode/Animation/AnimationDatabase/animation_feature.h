#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"
#include "animation_feature_weight.h"

class AnimationFeatures : public ISerializable
{
public:
  vector<vec3> features;
  AnimationFeatures();
  void set_feature(const string& name, vec3 feature);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

void pose_matching_norma_weights(AnimationFeaturesWeightsPtr weights_ptr);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);