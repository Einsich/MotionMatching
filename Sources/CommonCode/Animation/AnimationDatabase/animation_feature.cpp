#include "animation_feature.h"
#include <cmath>

AnimationFeatures::AnimationFeatures():
  features((int)AnimationFeaturesNode::Count, vec3(NAN)){}
size_t AnimationFeatures::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, path);
  size += write(os, features);
  return size;
}
size_t AnimationFeatures::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, path);
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
bool contains(const vector<AnimationTag> &target, const vector<AnimationTag> &set)
{
  //if (target.size() != set.size())
  //  return 0;
  for (AnimationTag tag1 : target)
  {
    bool exist = false;
    for (AnimationTag tag2 : set)
      if (tag2 == tag1)
      {
        exist = true;
        break;
      }
    if (!exist)
      return 0;
  }
  return 1;
}
float goal_matching_norma(const AnimationPathFeature &path, const vector<AnimationTag> &tags, const AnimationGoal &goal)
{
  float path_norma = 0.f, rotation_norma = 0.f;
  for (uint i = 0; i < AnimationPathFeature::PathLength; i++)
    path_norma += length(path.path[i] - goal.path.path[i]);
  path_norma *= weights->goal_path_weight;
  if (length(goal.path.path[0]) < 0.1f)
    rotation_norma = weights->goal_rotation * abs(goal.path.rotation - path.rotation);
  
  return (1.f - path_norma - rotation_norma + contains(goal.tags, tags) * weights->goal_tag_weight) * weights->goal_weight;
}

float next_cadr_norma(int cur_anim, int cur_cadr, int next_anim, int next_cadr, int clip_lenght)
{
  int d = (next_cadr - cur_cadr - 1 + clip_lenght) % clip_lenght ;
  if (next_anim == cur_anim && d < 8)
    return glm::clamp(1.f - d * 0.125f, 0.f, 1.f) * weights->next_cadr_weight;
  d -= clip_lenght;
  if (next_anim == cur_anim && d > -8)
    return glm::clamp(-1.f - d * 0.125f, -1.f, 0.f) * weights->next_cadr_weight;
  return 0;
}