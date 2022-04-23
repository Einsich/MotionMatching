#pragma once
#include "asset.h"
#include <assimp/scene.h>
#include "serialization/reflection.h"
class FBXMeta : public IAsset
{
  REFLECT(FBXMeta,
  (bool) (loaded),
  (vector<string>) (meshMetaData))
  void create_assets() const;
  void read_file_info(const filesystem::path &path);
public:
  virtual void load(const filesystem::path &path, bool reload, AssetStatus &status) override;
  
  virtual bool after_construct(const filesystem::path &path) override;
};
