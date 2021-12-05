#pragma once
#include "common.h"
#include <vector>
#include <array>
#include "3dmath.h"
#include "../animation_tag.h"
#include "serialization/serialization.h"

class NodeFeatures 
{
public:
  vector<vec3> nodes;
  vector<vec3> nodesVelocity;
  NodeFeatures();
  void set_feature(const string& name, vec3 feature);
};

struct TrajectoryPoint
{
  vec3 point;
  float rotation;
};

class AnimationTrajectory
{
public: 
  static const int PathLength = 3;
  static constexpr std::array<float, PathLength> timeDelays = { 0.15f, 0.3f, 0.5f};
  array<TrajectoryPoint, PathLength> trajectory;
};

struct FrameFeature final : public ISerializable
{
public:
  NodeFeatures features;
  AnimationTrajectory trajectory;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

struct AnimationGoal
{
  FrameFeature feature; 
  AnimationTags tags;
};

struct MatchingScores
{
  float pose, goal_tag, goal_rotation, goal_path;
  float full_score;
};
struct MotionMatchingSettings;

float pose_matching_norma(const NodeFeatures& feature1, const NodeFeatures& feature2, const MotionMatchingSettings &settings);

float goal_tag_norma(AnimationTags goal, AnimationTags clips_tag);
float rotation_norma(const AnimationTrajectory &path, const AnimationTrajectory &goal);
float goal_path_norma(const AnimationTrajectory &path, const AnimationTrajectory &goal);

float next_cadr_norma(int cur_anim, int cur_cadr, int next_anim, int next_cadr, int clip_lenght);

MatchingScores get_score(const FrameFeature& clip_feature, const FrameFeature &goal_feature, const MotionMatchingSettings &settings);

bool has_goal_tags(AnimationTags goal, AnimationTags clips_tag);
