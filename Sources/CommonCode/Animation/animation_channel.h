#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"

class AnimationCadr : public ISerializable
{
public:
  vec3 rootTranslationDelta;
  float rootRotationDelta;

  vec3 nodeTranslation;
  vector<quat> nodeRotation;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
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
AnimationCadr lerped_cadr(const AnimationCadr& cadr1, const AnimationCadr& cadr2, float t);

void change_pose_matching_norma_weights(AnimationFeaturesNode node, float weight);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);