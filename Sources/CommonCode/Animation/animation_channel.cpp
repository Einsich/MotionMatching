#include "animation_channel.h"
#include <cmath>

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


static float weights[(int)AnimationFeaturesNode::Count] = {1.f};
void change_pose_matching_norma_weights(AnimationFeaturesNode node, float weight)
{
  weights[(int)node] = weight;
}

void AnimationFeatures::set_feature(const string& name, vec3 feature)
{
  const static map<string, AnimationFeaturesNode> featureMap = {
    {"Hips", AnimationFeaturesNode::Hips},
    {"LeftHand", AnimationFeaturesNode::LeftHand},
    {"RightHand", AnimationFeaturesNode::RightHand},
    {"LeftFoot", AnimationFeaturesNode::LeftFoot},
    {"RightFoot", AnimationFeaturesNode::RightFoot}};
  auto it = featureMap.find(name);
  if (it != featureMap.end())
    features[(int)it->second] = feature;
}
float pose_matching_norma(const AnimationFeatures& feature1, const AnimationFeatures& feature2)
{
  float norma = 0.f;
  for (int i = 0; i < (int)AnimationFeaturesNode::Count; i++)
    norma += weights[i] * length(feature1.features[i] - feature2.features[i]);
  return norma;
}