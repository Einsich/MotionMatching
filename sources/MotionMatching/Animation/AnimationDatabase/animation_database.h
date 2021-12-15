#pragma once
#include <common.h>
#include <serialization/reflection.h>
#include <assimp/scene.h>
#include "animation_clip.h"
#include "../Test/animation_test.h"
#include <resources/asset.h>
#include <resources/fbx_importer.h>
#include "../AccelerationStruct/vp_tree.h"

class AnimationDataBase : public  IAsset
{
public:
  AnimationTreeData tree;
  REFLECT(AnimationDataBase,
    (vector<AnimationClip>) (clips),
    (Asset<FBXMeta>) (treeSource)
  )
  bool needForceReload;
  std::vector<AnimationTest> tests;
  std::vector<std::vector<float>> matchingScore;
  std::vector<VPTree<FrameFeature, FrameMetric>> vpTrees;

  int cadr_count() const;
  void save_runtime_parameters() const;
  void load_runtime_parameters();
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual bool edit() override; 
};

using AnimationDataBasePtr = Asset<AnimationDataBase>;