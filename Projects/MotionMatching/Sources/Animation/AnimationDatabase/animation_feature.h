#pragma once
#include "common.h"
#include <vector>
#include <set>
#include "Serialization/serialization.h"
#include "3dmath.h"
#include "animation_feature_weight.h"
#include "animation_trajectory.h"
#include "../animation_goal.h"

class AnimationFeatures final : public ISerializable
{
public:
  vector<vec3> features;
  AnimationFeatures();
  ~AnimationFeatures()=default;
  void set_feature(const string& name, vec3 feature);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
struct MatchingScores
{
  float pose, goal_tag, goal_rotation, goal_path, noise;
  float full_score;
};
void pose_matching_norma_weights(AnimationFeaturesWeightsPtr weights_ptr);
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2);

float goal_tag_norma(const vector<AnimationTag> &target, const vector<AnimationTag> &set);
float rotation_norma(const AnimationTrajectory &path, const AnimationGoal &goal);
float goal_path_norma(const AnimationTrajectory &path, const AnimationGoal &goal);

float next_cadr_norma(int cur_anim, int cur_cadr, int next_anim, int next_cadr, int clip_lenght);

MatchingScores get_score(const AnimationFeatures& feature1, const set<AnimationTag> &clip_tags, const AnimationFeatures& feature2,  const AnimationTrajectory &frame_trajectory, const AnimationGoal &goal);

bool has_goal_tags(const set<AnimationTag> &goal, const set<AnimationTag> &clips_tag);
