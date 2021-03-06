#pragma once
#include "../../common.h"
#include "../../Serialization/serialization.h"
#include <assimp/scene.h>
#include "animation_feature_weight.h"
#include "animation_clip.h"


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