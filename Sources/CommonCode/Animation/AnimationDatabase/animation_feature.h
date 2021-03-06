#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"
#include "animation_feature_weight.h"
#include "animation_path_feature.h"
#include "../animation_goal.h"

class AnimationFeatures : public ISerializable
{
public:
  AnimationPathFeature path;
  vector<vec3> features;
  AnimationFeatures();
  void set_feature(const string& name, vec3 feature);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

void pose_matching_norma_weights(AnimationFeaturesWeightsPtr weights_ptr);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);

float goal_matching_norma(const AnimationPathFeature &path, const vector<AnimationTag> &tags, const AnimationGoal &goal);

float next_cadr_norma(int cur_anim, int cur_cadr, int next_anim, int next_cadr, int clip_lenght);