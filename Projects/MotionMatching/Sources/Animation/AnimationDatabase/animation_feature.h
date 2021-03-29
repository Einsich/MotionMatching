#pragma once
#include "common.h"
#include <vector>
#include <set>
#include "Serialization/serialization.h"
#include "math.h"
#include "animation_feature_weight.h"
#include "animation_trajectory.h"
#include "../animation_goal.h"

class AnimationFeatures : public ISerializable
{
public:
  vector<vec3> features;
  AnimationFeatures();
  void set_feature(const string& name, vec3 feature);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
struct MatchingScores
{
  float pose, goal_tag, goal_rotation, goal_path, next_cadr, noise;
  float full_score, final_norma;
};
void pose_matching_norma_weights(AnimationFeaturesWeightsPtr weights_ptr);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);

float goal_tag_norma(const vector<AnimationTag> &target, const vector<AnimationTag> &set);
float rotation_norma(const AnimationTrajectory &path, const AnimationGoal &goal);
float goal_path_norma(const AnimationTrajectory &path, const AnimationGoal &goal);

float next_cadr_norma(int cur_anim, int cur_cadr, int next_anim, int next_cadr, int clip_lenght);

MatchingScores get_score(const AnimationFeatures& feature1, const AnimationFeatures& feature2,  const AnimationTrajectory &frame_trajectory, const AnimationGoal &goal);