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

AnimationFeatures::AnimationFeatures():
  nodes((int)AnimationFeaturesNode::Count, vec3(NAN)), nodesVelocity((int)AnimationFeaturesNode::Count, vec3(NAN)){}
size_t AnimationFeatures::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, nodes);
  size += write(os, nodesVelocity);
  return size;
}
size_t AnimationFeatures::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, nodes);
  size += read(is, nodesVelocity);
  return size;
}


void AnimationFeatures::set_feature(const string& name, vec3 feature)
{
  auto it = featureMap.find(name);
  if (it != featureMap.end())
    nodes[(int)it->second] = feature;
  
}
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2, const MotionMatchingSettings &settings)
{
  float pose_norma = 0.f, vel_norma = 0.f;
  for (int i = 0; i < (int)AnimationFeaturesNode::Count; i++)
  {
    pose_norma += settings.nodeWeights[i] * length(feature1.nodes[i] - feature2.nodes[i]);
    if (settings.velocityMatching)
      vel_norma += settings.velocitiesWeights[i] * length(feature1.nodesVelocity[i] - feature2.nodesVelocity[i]);
  }
  return settings.poseMatchingWeight * pose_norma + settings.velocityMatchingWeight * vel_norma;
}
bool has_goal_tags(const set<AnimationTag> &goal, const set<AnimationTag> &clips_tag)
{
  for (AnimationTag tag1 : goal)
    if (clips_tag.find(tag1) == clips_tag.end())
      return false;
  return true;
}
float goal_tag_norma(const set<AnimationTag> &goal, const set<AnimationTag> &clips_tag)
{
  //if (goal.size() == 0)
    return 0;
  float count = 0;
  for (AnimationTag tag1 : goal)
  {
    bool exist = clips_tag.find(tag1) != clips_tag.end();
    count += exist;
  }
  return -(count / goal.size());
}
float rotation_norma(const AnimationTrajectory &path, const AnimationGoal &goal)
{
  float rotation_norma = 0.f;
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float r = abs(path.trajectory[i].rotation - goal.path.trajectory[i].rotation);
    r -= (int)(r / PITWO)*PITWO;
    r = r > PI ? PITWO - r : r;
    rotation_norma += r;
  }
  return rotation_norma;
}
float goal_path_norma(const AnimationTrajectory &path, const AnimationGoal &goal)
{
  float path_norma = 0.f;
  float distScale = length(goal.path.trajectory[(int)AnimationTrajectory::PathLength - 1].point);
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
    path_norma += length((path.trajectory[i].point - goal.path.trajectory[i].point));
  return  path_norma / (0.1f + distScale);
}

MatchingScores get_score(const AnimationFeatures& feature1, const set<AnimationTag> &clip_tags, 
  const AnimationFeatures& feature2, const AnimationTrajectory &frame_trajectory, const AnimationGoal &goal,
  const MotionMatchingSettings &settings)
{
  MatchingScores score{0, 0, 0, 0, 0};
  score.pose = pose_matching_norma(feature1, feature2, settings);
  score.goal_path = goal_path_norma(frame_trajectory, goal) * settings.goalPathMatchingWeight;
  score.goal_rotation = rotation_norma(frame_trajectory, goal) * settings.goalRotationMatchingWeight;
  score.goal_tag = goal_tag_norma(goal.tags, clip_tags) * settings.goalFavourTagWeight;
  score.full_score = score.pose * settings.realism + (score.goal_path + score.goal_rotation);
  #define NAN_LOG(var)if (std::isnan(-var)) debug_error("NAN in %s", #var);

  NAN_LOG(score.pose)
  NAN_LOG(score.goal_path)
  NAN_LOG(score.goal_rotation)
  return score;
}
const std::string &get_tag_name(AnimationTag tag)
{
  #define ADD_TAG(TAG) #TAG
  static vector<string> tag_names = {
    TAGS
  };
  return tag_names[(int)tag];
  #undef ADD_TAG
}