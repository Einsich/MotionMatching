#pragma once
#include "../../common.h"
#include "../../Serialization/serialization.h"
#include "../AnimationTree/animation_tree.h"
#include "animation_feature.h"
#include "animation_cadr.h"

class AnimationClip: public ISerializable
{
private:

public:
  AnimationClip(){}
  AnimationClip(uint duration, float ticksPerSecond, const string &name, AnimationTreeData& tree,
  const map<string, vector<quat>>& quats, const map<string, vector<vec3>>& vecs);
  uint duration;
  float ticksPerSecond;
  string name;
  vector<AnimationCadr> cadres;
  vector<AnimationFeatures> features;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  AnimationCadr get_lerped_cadr(int cadr, float t) const;
};
class AnimationDataBase : public ISerializable
{
public:
  AnimationDataBase(aiNode *root);
  AnimationTreeData tree;
  vector<AnimationClip> clips;
  AnimationFeaturesWeightsPtr featureWeights;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  int cadr_count() const;
  void save_runtime_parameters();
  void load_runtime_parameters();
};
using AnimationClipPtr = shared_ptr<AnimationClip>;

using AnimationDataBasePtr = shared_ptr<AnimationDataBase>;