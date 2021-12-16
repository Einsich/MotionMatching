#include "animation_feature.h"
#include "animation_nodes.h"
#include "../settings.h"
#include <cmath>
#include <cstdlib>
#include <map>

#define FEATURE(node) {#node, (int)AnimationFeaturesNode::node}, {#node "Speed",(int)AnimationFeaturesNode::Count + (int)AnimationFeaturesNode::node}
    
std::map<std::string, int> featureMap = {
  FEATURE(Hips),
  FEATURE(LeftHand),
  FEATURE(RightHand),
  FEATURE(LeftToeBase),
  FEATURE(RightToeBase)
};
#undef FEATURE

NodeFeatures::NodeFeatures():
  nodes((int)AnimationFeaturesNode::Count, vec3(NAN)), nodesVelocity((int)AnimationFeaturesNode::Count, vec3(NAN)){}

size_t FrameFeature::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, features.nodes);
  size += write(os, features.nodesVelocity);
  size += write(os, trajectory.trajectory);
  return size;
}
size_t FrameFeature::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, features.nodes);
  size += read(is, features.nodesVelocity);
  size += read(is, trajectory.trajectory);
  return size;
}


void NodeFeatures::set_feature(const string& name, vec3 feature)
{
  auto it = featureMap.find(name);
  if (it != featureMap.end())
    nodes[(int)it->second] = feature;
  
}
float pose_matching_norma(const NodeFeatures& feature1, const NodeFeatures& feature2, const MotionMatchingSettings &settings)
{
  float pose_norma = 0.f, vel_norma = 0.f;
  for (int i = 0; i < (int)AnimationFeaturesNode::Count; i++)
  {
    pose_norma += settings.nodeWeights[i] * length(feature1.nodes[i] - feature2.nodes[i]);
    vel_norma += settings.velocityMatching ? settings.velocitiesWeights[i] * length(feature1.nodesVelocity[i] - feature2.nodesVelocity[i]) : 0.f;
  }
  return settings.poseMatchingWeight * pose_norma + settings.velocityMatchingWeight * vel_norma;
}
bool has_goal_tags(AnimationTags goal, AnimationTags clips_tag)
{
  return goal.tags == clips_tag.tags;
}

float goal_tag_norma(AnimationTags /* goal */, AnimationTags /* clips_tag */)
{
  return 0;
}

float rotation_norma(const AnimationTrajectory &path, const AnimationTrajectory &goal)
{
  float rotation_norma = 0.f;
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float r = abs(path.trajectory[i].rotation - goal.trajectory[i].rotation);
    r -= (int)(r / PITWO)*PITWO;
    r = r > PI ? PITWO - r : r;
    rotation_norma += r;
  }
  return rotation_norma;
}
float goal_path_norma(const AnimationTrajectory &path, const AnimationTrajectory &goal)
{
  float path_norma = 0.f;
  float distScale = length(goal.trajectory[(int)AnimationTrajectory::PathLength - 1].point);
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
    path_norma += length((path.trajectory[i].point - goal.trajectory[i].point));
  return  path_norma / (0.1f + distScale);
}

MatchingScores get_score(const FrameFeature& clip_feature, const FrameFeature& goal_feature,
  const MotionMatchingSettings &settings)
{
  MatchingScores score{0, 0, 0, 0, 0};
  score.pose = pose_matching_norma(clip_feature.features, goal_feature.features, settings);
  score.goal_path = goal_path_norma(clip_feature.trajectory, goal_feature.trajectory) * settings.goalPathMatchingWeight;
  score.goal_rotation = rotation_norma(clip_feature.trajectory, goal_feature.trajectory) * settings.goalRotationMatchingWeight;
  score.full_score = score.pose * settings.realism + (score.goal_path + score.goal_rotation);
  #define NAN_LOG(var)if (std::isnan(-var)) debug_error("NAN in %s", #var);

  //NAN_LOG(score.pose)
  //NAN_LOG(score.goal_path)
  //NAN_LOG(score.goal_rotation)
  return score;
}

float FrameMetric::distance(const FrameFeature& clip_feature, const FrameFeature &goal_feature) const
{
  return 
  pose_matching_norma(clip_feature.features, goal_feature.features, settings) * settings.realism
    +
  goal_path_norma(clip_feature.trajectory, goal_feature.trajectory) * settings.goalPathMatchingWeight
    +
  rotation_norma(clip_feature.trajectory, goal_feature.trajectory) * settings.goalRotationMatchingWeight;
}