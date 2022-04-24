#pragma once
#include <future>
#include <type_traits>
#include <string>
#include <string_view>
#include <istream>
#include <fstream>
#include <filesystem>
#include "application/time.h"
#include "manager/compile_time_string.h"
#include "serialization/serialization.h"
#include "data_block/data_block.h"

enum class AssetStatus
{
  NotLoaded,
  Loading,
  AsyncLoadingFinished,
  Loaded
};

class IAsset
{
public:
  virtual bool after_construct(const filesystem::path &)
  {
    return false;
  }
  //called when this resource really needed - load data from disk 
  virtual void load(const filesystem::path &path, bool reload, AssetStatus &status) = 0;
  //dispose resource - called on application end, or when we can unload resource
  virtual void before_save()
  {

  }
  //this fuction need to edit resource and call reload if it changed
  virtual bool edit()
  {
    return false;
  }
  virtual string asset_name(const filesystem::path &path)
  {
    return path.stem().string();
  }
  virtual bool require_immediate_load() const
  {
    return false;
  }
};


template<typename T>
class Asset;

template<typename T>
Asset<T> create_asset_by_id(const string &name);


class AssetStub : public  IAsset
{
public:
  virtual void load(const filesystem::path &, bool, AssetStatus &) override{}
  virtual void before_save() override{}
  virtual bool edit() override{return false;}
};

bool register_asset(const string &assetName, const string_view &typeName, const Asset<AssetStub> &asset);

template<typename T>
struct AssetImplementation
{
  filesystem::path path;
  string name;
  AssetStatus status;
  bool edited, isCopy, userPathUsage;
  T asset;
  AssetImplementation() = default;
  AssetImplementation(const filesystem::path &path, const string &name,
    bool loaded, bool edited, bool isCopy, bool userPathUsage, const T &asset):
  path(path), name(name),
  status(loaded ? AssetStatus::Loaded : AssetStatus::NotLoaded),
  edited(edited), isCopy(isCopy), userPathUsage(userPathUsage),
  asset(asset)
  {    }
template<typename ...Args>
  AssetImplementation(const filesystem::path &path, const string &name,
    bool loaded, bool edited, bool isCopy, bool userPathUsage,  Args &&...args):
  path(path), name(name),
  status(loaded ? AssetStatus::Loaded : AssetStatus::NotLoaded),
  edited(edited), isCopy(isCopy), userPathUsage(userPathUsage),
  asset(args...)
  {    }

  void try_load()
  {
    if (status != AssetStatus::Loaded)
    {
      //TimeScope loadingScope(name.c_str());
      asset.load(path, false, status);
    }
  }
  void reload()
  {
    if (status == AssetStatus::Loaded)
      asset.load(path, true, status);
  }
};
template<typename T>
class Asset final: public ISerializable
{
public:
  static_assert(std::is_base_of<IAsset, T>::value);

  AssetImplementation<T> *asset;
  //create asset and init them from .meta file or only default value
  explicit Asset():asset(nullptr){}
  Asset(nullptr_t):asset(nullptr){}
  ~Asset() = default;
  Asset<T>& operator=(const Asset<T>& other)
  {
    asset = other.asset;
    return *this;
  }
  explicit Asset(AssetImplementation<T> *asset):
    asset(asset){}
  
  Asset(const filesystem::path &path_or_name) :
  asset(new AssetImplementation<T>(
    path_or_name, "",
    false, false, false, true, T()))
  {

    asset->name = asset->asset.asset_name(path_or_name);
    constexpr const string_view &typeName = nameOf<T>::value;
    register_asset(asset->name, typeName, *this);
  }
    Asset(const T &instance) :
    asset(new AssetImplementation<T>(
    "", "",
    true, false, false, true, instance))
  {

  }
  Asset(const DataBlock &blk) :
  asset(new AssetImplementation<T>(
    root_path(blk.get<std::string>("path", "")), "",
    false, false, false, false, T()))
  {
    asset->name = asset->asset.asset_name(asset->path);
    if (!blk.get<bool>("new_asset", false))
      *this = create_asset_by_id<T>(asset->name);
    else
      asset->try_load();

    if (!loaded() && !asset->path.empty() && asset->asset.require_immediate_load())
      asset->try_load();
  }
  template<typename ...Args>
  Asset(const filesystem::path &path_or_name, Args &&...args) :
  asset(new AssetImplementation<T>(
    path_or_name, "",
    true, false, false, true, args...))
  {

    asset->name = asset->asset.asset_name(path_or_name);
    constexpr const string_view &typeName = nameOf<T>::value;
    register_asset(asset->name, typeName, *this);
  }
  template<typename U>
  operator Asset<U>() const
  {
    return *((Asset<U>*)this);
  }
  explicit operator bool() const
  {
    return asset != nullptr && (asset->status == AssetStatus::Loaded || asset->name != "");
  }
  T* operator->()
  {
    return &asset->asset;
  }
  const T* operator->() const 
  {
    return &asset->asset;
  }
  T* get()
  {
    return &asset->asset;
  }
  const T* get() const 
  {
    return &asset->asset;
  }
  T &operator*()
  {
    return asset->asset;
  }
  const T &operator*() const 
  {
    return asset->asset;
  }
  void load()
  {
    asset->try_load();
  }
  bool try_load()
  {
    if (asset->status != AssetStatus::Loaded)
      asset->try_load();
    return asset->status == AssetStatus::Loaded;
  }
  void reload()
  {
    asset->reload();
  }
  void save(ofstream &file)
  {
    if (asset)
    {
      write(file, asset->name);
      write(file, asset->asset);
    }
  }
  void save() const
  {
    if (asset)
    {
      asset->asset.before_save();
      if (!asset->userPathUsage)
      { 
        ofstream file(asset->path, ios::binary);
        if (!file.fail())
        {
          write(file, asset->name);
          write(file, asset->asset);
        }
      }
    }
  }
  bool edit()
  {
    bool edited = asset->asset.edit();
    asset->edited |= edited;
    return edited;
  }
  bool loaded() const
  {
    return asset->status == AssetStatus::Loaded;
  }
  bool edited() const
  {
    return asset->edited;
  }
  bool is_copy() const
  {
    return asset->isCopy;
  }
  const string& asset_name() const
  {
    static string dummy = "null";
    return asset ? asset->name : dummy;
  }
  const filesystem::path &asset_path() const
  {
    static filesystem::path defPath = "null";
    return asset ? asset->path : defPath;
  }
  virtual size_t serialize(std::ostream& os) const override
  {
    return write(os, asset ? asset->name : "null");
  }
  virtual size_t deserialize(std::istream& is) override
  {
    string name;
    size_t size = read(is, name);
    if (name != "null" && name != "")
      *this = create_asset_by_id<T>(name);
    else 
      *this = Asset<T>();
    return size;
  }
  Asset<T> copy() const
  {
    if (asset)
    {
      Asset<T> a;
      a.asset = new AssetImplementation<T>();
      *a.asset = *asset;
      a.asset->isCopy = true;
      return a;
    }
    else
      return Asset<T>();
  }
  void copy(const Asset<T> &other)
  {
    if (asset && other.asset)
    {
      asset->asset = other.asset->asset;
    }
  }
};