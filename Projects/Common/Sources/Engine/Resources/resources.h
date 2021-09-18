#pragma once
#include <functional>
#include <map>
#include <filesystem>
#include "asset.h"

struct ResourceMap
{
  string name;
  bool metaDataAsset;
  std::map<std::string, Asset<AssetStub>> resources;
  std::function<void(const filesystem::path&)> createAsset;
  std::function<void(const filesystem::path&, const Asset<AssetStub>&)> createCopyAsset;
  std::function<void(const Asset<AssetStub>&)> saveAsset;
  std::function<bool(Asset<AssetStub>&)> editAsset;
  std::function<void(Asset<AssetStub>&, bool)> loadAsset;
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

template<typename T, bool meta_data_asset>
Asset<T> create_asset(const filesystem::path &path)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  const string assetName = get_asset_name(path);
  auto it = resourcesMap.resources.find(assetName);
  if (it == resourcesMap.resources.end())
  {
    Asset<T> asset(path, meta_data_asset);
    resourcesMap.resources.try_emplace(assetName, asset);
    return asset;
  }
  else if (!it->second)
  {
    it->second = Asset<T>(path, meta_data_asset);
  }
  return it->second;
}
template<typename T>
bool register_asset(const string &assetName, const Asset<T> &asset)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  auto it = resourcesMap.resources.find(assetName);
  if (it == resourcesMap.resources.end())
  {
    resourcesMap.resources.try_emplace(assetName, asset);
    return true;
  }
  return false;
}

template<typename T>
Asset<T> create_copy_asset(const filesystem::path &path, const Asset<AssetStub>& dest)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  const string assetName = get_asset_name(path);
  auto it = resourcesMap.resources.find(assetName);
  if (it == resourcesMap.resources.end())
  {
    Asset<T> asset(path, dest);
    resourcesMap.resources.try_emplace(assetName, asset);
    return asset;
  }
  return it->second;
}
template<typename T>
void save_asset(const Asset<AssetStub> &asset)
{
  static_cast<const Asset<T>>(asset).save();
}
template<typename T>
bool edit_asset(Asset<AssetStub> &asset)
{
  return static_cast<Asset<T>>(asset).edit();
}
template<typename T>
void load_asset(Asset<AssetStub> &asset, bool async)
{
  return static_cast<Asset<T>>(asset).load(async);
}
template<typename T>
void reload_asset(Asset<AssetStub> &asset)
{
  return static_cast<Asset<T>>(asset).reload();
}
template<typename T>
struct ResourceRegister
{
  template<bool metaDataAsset>
  void add_asset(const string_view &assetName)
  {
    Resources::instance().assets.try_emplace(assetName, 
    ResourceMap{string(assetName), metaDataAsset, {}, create_asset<T, metaDataAsset>, create_copy_asset<T>, save_asset<T>, edit_asset<T>, load_asset<T>, reload_asset<T>});

  }
  ResourceRegister(const std::vector<std::string> &extensions)
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    for (const std::string &extension : extensions)
      Resources::instance().extToAssetName.try_emplace(extension, assetName);
    add_asset<true>(assetName);
  }
  ResourceRegister()
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    Resources::instance().extToAssetName.try_emplace("."+string(assetName), assetName);
    add_asset<false>(assetName);
  }
};
template<typename T>
Asset<T> get_resource(const string &name)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName].resources;
  auto it = resourcesMap.find(name);
  if (it == resourcesMap.end())
  {
    debug_error("Can't find %s asset %s", typeName.data(), name.c_str());
    return Asset<T>();
  }
  return it->second;
}
void create_all_resources_from_metadata();

//creating asset from file
// if path.ext from {.ext1, .ext2 ... } create Asset<T> and save into path.ext.meta 
//creating asset from code
//register Asset<T> creator with T

// if path.ext.meta from {.ext1, .ext2 ... } create Asset<T> and pass this file into constuctor

//register T like {.ext1, .ext2 ... } with reader Asset<T> 
//register 
