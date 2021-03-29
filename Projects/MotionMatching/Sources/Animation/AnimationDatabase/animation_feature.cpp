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
    norma += weights->weights[i] * length(feature1.features[i] - feature2.features[i]);
  return weights->norma_function_weight * norma;
}
float goal_tag_norma(const set<AnimationTag> &goal, const set<AnimationTag> &clips_tag)
{
  if (goal.size() != clips_tag.size())
    return 0;
  for (AnimationTag tag1 : goal)
  {
    bool exist = clips_tag.find(tag1) != clips_tag.end();
    if (!exist)
      return 0;
  }
  return weights->goal_tag_weight;
}
float rotation_norma(const AnimationTrajectory &path, const AnimationGoal &goal)
{
  float rotation_norma = 0.f;
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    rotation_norma += abs(path.trajectory[i].rotation - goal.path.trajectory[i].rotation);
  }
  return weights->goal_rotation * rotation_norma;
}
float goal_path_norma(const AnimationTrajectory &path, const AnimationGoal &goal)
{
  float path_norma = 0.f;
  for (uint i = 0; i < AnimationTrajectory::PathLength; i++)
    path_norma += length(path.trajectory[i].point - goal.path.trajectory[i].point);
  return weights->goal_path_weight * path_norma;
}

MatchingScores get_score(const AnimationFeatures& feature1, const AnimationFeatures& feature2, const AnimationTrajectory &frame_trajectory, const AnimationGoal &goal)
{
  MatchingScores score{0};
  score.pose = pose_matching_norma(feature1, feature2);
  score.goal_path = goal_path_norma(frame_trajectory, goal);
  score.goal_rotation = rotation_norma(frame_trajectory, goal);
  score.noise = (1.f * std::rand() / RAND_MAX) * weights->noise_scale;
  score.full_score = score.pose + score.goal_path + score.goal_rotation + score.noise;
  //score.final_norma = score.full_score > 0.f ? 1.f / score.full_score : 0;
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