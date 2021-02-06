#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"


enum class AnimationFeaturesNode
{
  Hips,
  LeftHand,
  RightHand,
  LeftFoot,
  RightFoot,
  Count
};
class AnimationFeatures : public ISerializable
{
public:
  vector<vec3> features;
  AnimationFeatures();
  void set_feature(const string& name, vec3 feature);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

void change_pose_matching_norma_weights(AnimationFeaturesNode node, float weight);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);