#include "animation_feature_weight.h"


#define FEATURE(F) {#F, (int)AnimationFeaturesNode::F}, {#F "Speed",(int)AnimationFeaturesNode::Count + (int)AnimationFeaturesNode::F}
AnimationFeaturesWeights::AnimationFeaturesWeights():
norma_function_weight(1.f), goal_path_weight(1.f), goal_rotation(1.f), goal_tag_weight(1.f), y_norma_scale(1.f),
noise_scale(0.1f),
debug_scale(1.f),
animation_lerp(1.f),
weights(2 * (int)AnimationFeaturesNode::Count, 1.f), featureMap({
    FEATURE(Hips),
    FEATURE(LeftHand),
    FEATURE(RightHand),
    FEATURE(LeftToeBase),
    FEATURE(RightToeBase)})
{}
size_t AnimationFeaturesWeights::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, featureMap.size());
  for (const auto &p : featureMap)
  {
    size += write(os, p.first);
    size += write(os, weights[(int)p.second]);
  }
  size += write(os, norma_function_weight);
  size += write(os, goal_path_weight);
  size += write(os, goal_tag_weight);
  size += write(os, noise_scale);
  size += write(os, goal_rotation);
  size += write(os, y_norma_scale);
  size += write(os, animation_lerp);
  return size;
}
size_t AnimationFeaturesWeights::deserialize(std::istream& is)
{
  size_t size = 0;
  size_t n = 0;
  size += read(is, n);
  for (uint i = 0; i < n; i++)
  {
    string name; 
    float weight;
    size += read(is, name);
    size += read(is, weight);
    if (featureMap.find(name) != featureMap.end())
      weights[(int)featureMap[name]] = weight;
  }
  size += read(is, norma_function_weight);
  size += read(is, goal_path_weight);
  size += read(is, goal_tag_weight);
  size += read(is, noise_scale);
  size += read(is, goal_rotation);
  size += read(is, y_norma_scale);
  size += read(is, animation_lerp);
  return size;
}