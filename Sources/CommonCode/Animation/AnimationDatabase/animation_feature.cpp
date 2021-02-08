#include "animation_feature.h"
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
  return 1.f - weights->norma_function_weight * norma;
}