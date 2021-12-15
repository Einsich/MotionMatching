#include "resources.h"

template<typename T, bool meta_data_asset, bool user_path_usage>
Asset<T> create_new_asset(const filesystem::path &path)
{
  assert(!user_path_usage);//can create assets only with Asset<T>(user_path)

  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  AssetImplementation<T> *asset = new AssetImplementation<T>();

  asset->name = asset->asset.asset_name(path);
  asset->path = meta_data_asset ? path.string() + ".meta" : path;
  asset->edited = true;

  const string &assetName = asset->name;
  const filesystem::path &pathToAsset = asset->path;  
  {
    ofstream file(pathToAsset, ios::binary);//create file
  }
  ifstream file(pathToAsset, ios::binary);
  if (file.fail())
  {
    debug_error("file %s should exists when create asset", pathToAsset.string().c_str());
    delete asset;
    return Asset<T>();
  }
  auto it = resourcesMap.resources.find(assetName);

  asset->userPathUsage = user_path_usage;

  if (it == resourcesMap.resources.end())
  {
    Asset<T> asset_(asset);
    return resourcesMap.resources.try_emplace(assetName, asset_).first->second;
  }
  else if (!it->second)
  {
    return it->second = Asset<T>(asset);
  }
  else
  {
    debug_error("create asset %s more than 1 time", assetName.c_str());
    delete asset;
  }
  return it->second;
}

template<typename T, bool meta_data_asset, bool user_path_usage>
Asset<T> create_copy_asset(const filesystem::path &path, const Asset<AssetStub> &other)
{
  Asset<T> asset = create_new_asset<T, meta_data_asset, user_path_usage>(path);
  asset.copy(other);
  return asset;
}

template<typename T, bool meta_data_asset, bool user_path_usage>
Asset<T> create_exists_asset(const filesystem::path &path)
{
  assert(!user_path_usage);//can create assets only with Asset<T>(user_path)

  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  const filesystem::path &pathToAsset = meta_data_asset ? path.string() + ".meta" : path;

  ifstream file(pathToAsset, ios::binary);
  string name;
  if (!file.fail())
  {
    read(file, name);
  }
  else
  {
    debug_error("can't open file %s when load exists asset", pathToAsset.string().c_str());
    return Asset<T>();
  }
  name = path.stem().string();
 
  auto it = resourcesMap.resources.find(name);

  if (it == resourcesMap.resources.end())
  {
    AssetImplementation<T> *asset = new AssetImplementation<T>();

    asset->path = pathToAsset;
    asset->name = asset->asset.asset_name(pathToAsset);
    asset->userPathUsage = user_path_usage;
    read(file, asset->asset);
    asset->asset.after_load();
    Asset<T> asset_(asset);
    return resourcesMap.resources.try_emplace(asset->name, asset_).first->second;
  }
  else
  {
    Asset<T> asset = it->second;
    if (!it->second)
    {
      asset.asset->path = pathToAsset;
      asset.asset->name = asset.asset->asset.asset_name(pathToAsset);
      asset.asset->userPathUsage = user_path_usage;
      read(file, asset.asset->asset);
      asset.asset->asset.after_load();
    }
    else
    {
      debug_error("create asset %s more than 1 time", asset.asset->asset.asset_name(pathToAsset).c_str());
    }
    return asset;
  }
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
  else
  {
    it->second.copy(asset);
    return true;
  }
  return false;
}

template<typename T>
Asset<T> create_asset_by_id(const string &name)
{
  constexpr const string_view &typeName = nameOf<T>::value;
  auto &resourcesMap = Resources::instance().assets[typeName];
  auto it = resourcesMap.resources.find(name);
  if (it == resourcesMap.resources.end())
  {
    Asset<T> asset(new AssetImplementation<T>());

    return resourcesMap.resources.try_emplace(name, asset).first->second;
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
void load_asset(Asset<AssetStub> &asset)
{
  return static_cast<Asset<T>>(asset).load();
}
template<typename T>
void reload_asset(Asset<AssetStub> &asset)
{
  return static_cast<Asset<T>>(asset).reload();
}
template<typename T, bool userPathUsage = false>
struct ResourceRegister
{
  template<bool metaDataAsset, bool userPathUsage_>
  void add_asset(const string_view &assetName)
  {
    Resources::instance().assets.try_emplace(assetName, 
    ResourceMap{string(assetName), metaDataAsset, {},
      create_new_asset<T, metaDataAsset, userPathUsage_>, create_exists_asset<T, metaDataAsset, userPathUsage_>, 
      create_asset_by_id<T>, create_copy_asset<T, metaDataAsset, userPathUsage_>, 
      save_asset<T>, edit_asset<T>, load_asset<T>, reload_asset<T>});
  }
  ResourceRegister(const std::vector<std::string> &extensions)
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    for (const std::string &extension : extensions)
      Resources::instance().extToAssetName.try_emplace(extension, assetName);
    add_asset<true, userPathUsage>(assetName);
  }
  ResourceRegister()
  {
    constexpr const string_view &assetName = nameOf<T>::value;
    Resources::instance().extToAssetName.try_emplace("."+string(assetName), assetName);
    add_asset<false, userPathUsage>(assetName);
  }
};