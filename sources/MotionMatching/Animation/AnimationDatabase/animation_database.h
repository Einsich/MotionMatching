#pragma once
#include <common.h>
#include <serialization/reflection.h>
#include <assimp/scene.h>
#include "animation_clip.h"
#include <resources/asset.h>
#include <resources/fbx_importer.h>
#include "../AccelerationStruct/vp_tree.h"
#include "../AccelerationStruct/cover_tree.h"

class AnimationDataBase : public  IAsset
{
public:
  AnimationTreeData tree;
  REFLECT(AnimationDataBase,
    (vector<AnimationClip>) (clips),
    (Asset<FBXMeta>) (treeSource),
    (vector<string>) (tagsNames)
  )
  void acceleration_structs(bool check_existance = false);
  bool needForceReload;
  std::vector<std::vector<float>> matchingScore;

  std::vector<VPTree> vpTrees;
  std::vector<CoverTree> coverTrees;

  int cadr_count() const;
  virtual void load(const filesystem::path &path, bool reload, AssetStatus &status) override;
  virtual bool edit() override; 
};

using AnimationDataBasePtr = Asset<AnimationDataBase>;