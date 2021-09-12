#pragma once
#include "asset.h"
#include <assimp/scene.h>
#include "Serialization/reflection.h"
class FBXMeta : IAsset
{
  REFLECT(FBXMeta,
  (bool) (loaded),
  (vector<string>) (meshMetaData))
  void create_assets();
public:
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual void free() override;
  virtual bool edit() override;
};
