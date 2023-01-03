#pragma once
#include <functional>
#include <map>
#include <filesystem>
#include "asset.h"

struct ResourceMap
{
  std::string name;
  bool metaDataAsset;
  std::map<std::string, Asset<AssetStub>> resources;
  std::function<void(const filesystem::path&)> createNewAsset;
  std::function<void(const filesystem::path&)> createExistsAsset;
  std::function<void(const std::string&)> createAssetById; 
  std::function<void(const filesystem::path&, const Asset<AssetStub>&)> createCopyAsset;
  std::function<void(const Asset<AssetStub>&)> saveAsset;
  std::function<bool(Asset<AssetStub>&)> editAsset;
  std::function<void(Asset<AssetStub>&)> loadAsset;
  std::function<void(Asset<AssetStub>&)> reloadAsset;
};
class Resources
{
public:
  //type, name, cast to Asset<T>
  std::map<std::string_view, ResourceMap> assets;
  std::map<std::string, std::string_view> extToAssetName;
  static Resources &instance()
  {
    static Resources instance;
    return instance;
  }
};

template<typename T>
Asset<T> get_resource(const std::string &name)
{
  constexpr const std::string_view &typeName = nameOf<T>::value;
  for (auto &assetStub : Resources::instance().assets[typeName].resources)
  {
    const Asset<T> &asset = assetStub.second;
    if (asset.asset_name() == name)
      return asset;
  }
  debug_error("Can't find %s asset %s", typeName.data(), name.c_str());
  return Asset<T>();
}
