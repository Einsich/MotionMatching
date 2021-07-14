#pragma once
#include <functional>
#include <map>
#include <filesystem>
#include "asset.h"

struct ResourceMap
{
  bool metaDataAsset;
  std::map<std::string, Asset<AssetStub>> resources;
  std::function<void(const filesystem::path&)> createAsset;
  std::function<void(const Asset<AssetStub>&)> saveAsset;
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
void create_asset(const filesystem::path &path)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  Asset<T> asset(path);
  Asset<AssetStub> *stub = (Asset<AssetStub>*)&asset;
  resourcesMap.resources.try_emplace(path.filename().string(), *stub);
}

template<typename T>
void save_asset(const Asset<AssetStub> &asset)
{
  ((const Asset<T> *)(&asset))->save();
}
template<typename T>
struct ResourceRegister
{
  ResourceRegister(const std::vector<std::string> &extensions)
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    for (const std::string &extension : extensions)
      Resources::instance().extToAssetName.try_emplace(extension, assetName);
      
    Resources::instance().assets.try_emplace(assetName, ResourceMap{true, {}, create_asset<T>, save_asset<T>});
  }
  ResourceRegister()
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    Resources::instance().assets.try_emplace(assetName, ResourceMap{false, {}, create_asset<T>, save_asset<T>});
  }
};
template<typename T>
Asset<T> get_resource(const string &name)
{
  constexpr string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName].resources;
  auto it = resourcesMap.find(name);
  if (it == resourcesMap.end())
  {
    debug_error("Can't find %s asset %s", typeName.data(), name.c_str());
    return;
  }
  return static_cast<Asset<T>>(it->second);
}
void create_all_resources_from_metadata();

//creating asset from file
// if path.ext from {.ext1, .ext2 ... } create Asset<T> and save into path.ext.meta 
//creating asset from code
//register Asset<T> creator with T

// if path.ext.meta from {.ext1, .ext2 ... } create Asset<T> and pass this file into constuctor

//register T like {.ext1, .ext2 ... } with reader Asset<T> 
//register 
