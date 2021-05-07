#include "animation_feature.h"
#include <cmath>
#include <cstdlib>

AnimationFeatures::AnimationFeatures():
  features((int)AnimationFeaturesNode::Count, vec3(NAN)){}
size_t AnimationFeatures::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, features);
  return size;
}
size_t AnimationFeatures::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, features);
  return size;
}


static AnimationFeaturesWeightsPtr weights = make_shared<AnimationFeaturesWeights>();
void pose_matching_norma_weights(AnimationFeaturesWeightsPtr weights_ptr)
{
  weights = weights_ptr;
}

void AnimationFeatures::set_feature(const string& name, vec3 feature)
{
  auto it = weights->featureMap.find(name);
  if (it != weights->featureMap.end())
    features[(int)it->second] = feature;
  
}
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2)
{
  float norma = 0.f;
  for (int i = 0; i < (int)AnimationFeaturesNode::Count; i++)
    norma += weights->weights[i] * length2(feature1.features[i] - feature2.features[i]);
  return weights->norma_function_weight * norma;
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
  return -weights->goal_tag_weight * (count / goal.size());
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
  return weights->goal_rotation * rotation_norma;
}
float goal_path_norma(const AnimationTrajectory &path, const AnimationGoal &goal)
{
  float path_norma = 0.f;
  float distScale = length(goal.path.trajectory[(int)AnimationTrajectory::PathLength - 1].point);
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
    path_norma += length((path.trajectory[i].point - goal.path.trajectory[i].point) * vec3(1, weights->y_norma_scale, 1));
  return weights->goal_path_weight * path_norma / (0.1f + distScale);
}

MatchingScores get_score(const AnimationFeatures& feature1, const set<AnimationTag> &clip_tags, 
  const AnimationFeatures& feature2, const AnimationTrajectory &frame_trajectory, const AnimationGoal &goal)
{
  MatchingScores score{0, 0, 0, 0, 0, 0};
  score.pose = pose_matching_norma(feature1, feature2);
  score.goal_path = goal_path_norma(frame_trajectory, goal);
  score.goal_rotation = rotation_norma(frame_trajectory, goal);
  score.goal_tag = goal_tag_norma(goal.tags, clip_tags);
  score.noise = (1.f * std::rand() / RAND_MAX) * weights->noise_scale;
  score.full_score = score.pose + score.goal_path + score.goal_rotation + score.noise;
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