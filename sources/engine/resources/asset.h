#pragma once
#include <future>
#include <type_traits>
#include <string>
#include <string_view>
#include <istream>
#include <fstream>
#include <filesystem>
#include "manager/compile_time_string.h"
#include "serialization/serialization.h"
#include "data_block/data_block.h"

class IAsset
{
public:
  virtual void after_load()
  {

  }
  //called when this resource really needed - load data from disk 
  virtual void load(const filesystem::path &path, bool reload) = 0;
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
};


template<typename T>
class Asset;

template<typename T>
Asset<T> create_asset_by_id(const string &name);

template<typename T>
bool register_asset(const string &assetName, const Asset<T> &asset);

class AssetStub : public  IAsset
{
public:
  virtual void load(const filesystem::path &, bool) override{}
  virtual void before_save() override{}
  virtual bool edit() override{return false;}
};


template<typename T>
struct AssetImplementation
{
  filesystem::path path;
  string name;
  bool loaded, edited, isCopy, userPathUsage;
  T asset;
  AssetImplementation() = default;
  AssetImplementation(const filesystem::path &path, const string &name,
    bool loaded, bool edited, bool isCopy, bool userPathUsage, const T &asset):
  path(path), name(name),
  loaded(loaded), edited(edited), isCopy(isCopy), userPathUsage(userPathUsage),
  asset(asset)
  {    }
template<typename ...Args>
  AssetImplementation(const filesystem::path &path, const string &name,
    bool loaded, bool edited, bool isCopy, bool userPathUsage,  Args &&...args):
  path(path), name(name), 
  loaded(loaded), edited(edited), isCopy(isCopy), userPathUsage(userPathUsage),
  asset(args...)
  {    }

  void load()
  {
    if (!loaded)
    {
      asset.load(path, false);
      loaded = true;
    }
  }
  void reload()
  {
    if (loaded)
      asset.load(path, true);
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
    register_asset(asset->name, *this);
  }
  Asset(const DataBlock &blk) :
  asset(new AssetImplementation<T>(
    blk.get<std::string>("path", ""), "",
    false, false, false, false, T()))
  {
    asset->name = asset->asset.asset_name(asset->path);
    *this = create_asset_by_id<T>(asset->name);
  }
  template<typename ...Args>
  Asset(const filesystem::path &path_or_name, Args &&...args) :
  asset(new AssetImplementation<T>(
    path_or_name, "",
    true, false, false, true, args...))
  {

    asset->name = asset->asset.asset_name(path_or_name);
    register_asset(asset->name, *this);
  }
  template<typename U>
  operator Asset<U>() const
  {
    return *((Asset<U>*)this);
  }
  explicit operator bool() const
  {
    return asset != nullptr && asset->name != "";
  }
  T* operator->()
  {
    if (!asset->loaded)
      asset->load();
    return &asset->asset;
  }
  const T* operator->() const 
  {
    if (!asset->loaded)
      asset->load();
    return &asset->asset;
  }
  T &operator*()
  {
    if (!asset->loaded)
      asset->load();
    return asset->asset;
  }
  const T &operator*() const 
  {
    if (!asset->loaded)
      asset->load();
    return asset->asset;
  }
  void load()
  {
    asset->load();
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
    return asset->loaded;
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