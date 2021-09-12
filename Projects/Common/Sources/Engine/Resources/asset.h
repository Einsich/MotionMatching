#pragma once
#include <future>
#include <type_traits>
#include <string>
#include <string_view>
#include <istream>
#include <fstream>
#include <filesystem>
#include "ecs/manager/compile_time_string.h"
#include "Serialization/serialization.h"
class IAsset
{
  //called when this resource really needed - load data from disk 
  virtual void load(const filesystem::path &path, bool reload) = 0;
  //dispose resource - called on application end, or when we can unload resource
  virtual void free() = 0;
  //this fuction need to edit resource and call reload if it changed
  virtual bool edit() = 0;
};

/*
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual void free() override;
  virtual bool edit() override;
*/

string get_asset_name(const filesystem::path &path);

template<typename T>
class Asset;

template<typename T>
Asset<T> create_asset(const filesystem::path &path);

template<typename T>
bool register_asset(const string &assetName, const Asset<T> &asset);
class AssetStub : IAsset
{
public:
  virtual void load(const filesystem::path &, bool) override{}
  virtual void free() override{}
  virtual bool edit() override{return false;}
};
template<typename T>
class Asset final: public ISerializable
{
  static_assert(std::is_base_of<IAsset, T>::value);
  struct ResourceInfo
  {
    filesystem::path path;
    bool loading, loaded, edited, isCopy;
    T asset;
    void load()
    {
      if (!loaded && !loading)
      {
        loading = true;
        asset.load(path, false);
        loaded = true;
      }
    }
    void load_async()
    {
      if (!loaded && !loading)
      {
        loading = true;      
        auto asyncState = std::async(std::launch::async, [&](){asset.load(path, false);});
      }
    }
    void reload()
    {
      if (loaded)
        asset.load(path, true);
    }
    void save()
    {
      if (loaded)
        asset.save(path);
    }
    void save_async()
    {
      if (loaded)
        std::async(std::launch::async, &IAsset::save, iasset, path);
    }
  };
  ResourceInfo *asset;
public:
  ResourceInfo *resource() const
  {
    return asset;
  }
  //create asset and init them from .meta file or only default value
  Asset():asset(nullptr){}
  Asset(nullptr_t ):asset(nullptr){}
  ~Asset() = default;
  Asset<T>& operator=(const Asset<T>& other)
  {
    asset = other.asset;
    return *this;
  }
  Asset(const filesystem::path &resource_path)
  {  
    ifstream file(resource_path, ios::binary);
    if (!file.fail())
    {
      asset = new ResourceInfo{resource_path, false, false, false, false, T()};
      read(file, asset->asset);
    }
    else
      asset = nullptr;
  }
  Asset(const filesystem::path &resource_path, const Asset<T> &other) :
  asset(new ResourceInfo{resource_path, other.asset->loading, other.asset->loaded, 
        other.asset->edited, other.asset->isCopy, other.asset->asset})
  {
  }
  
  template<typename ...Args>
  Asset(const filesystem::path &path_or_name, bool need_save, Args &&...args) :
  asset(new ResourceInfo{path_or_name, true, true, need_save, false, T(args...)})
  {
    register_asset(path_or_name.string(), *this);
  }
  template<typename U>
  operator Asset<U>() const
  {
    return *((Asset<U>*)this);
  }
  operator bool() const
  {
    return asset != nullptr;
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
  void load(bool async = false)
  {
    if (async)
      asset->load_async();
    else
      asset->load();
  }
  void reload()
  {
    asset->reload();
  }
  void save() const
  {
    if (asset)
      asset->asset.free();
    ofstream file(asset->path, ios::binary);
    if (!file.fail())
    {
      write(file, asset->asset);
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
  string asset_name() const
  {
    return get_asset_name(asset->path);
  }
  const filesystem::path &asset_path() const
  {
    static filesystem::path defPath = "";
    return asset ? asset->path : defPath;
  }
  virtual size_t serialize(std::ostream& os) const override
  {
    return write(os, asset ? asset->path.string() : "null");
  }
  virtual size_t deserialize(std::istream& is) override
  {
    string path;
    size_t size = read(is, path);
    if (path != "null" && path != "")
      *this = create_asset<T>(filesystem::path(path));
    else 
      *this = Asset<T>();
    return size;
  }
  Asset<T> copy() const
  {
    if (asset)
    {
      Asset<T> a;
      // loading, loaded, edited, isCopy;
      a.asset = new ResourceInfo{asset->path, asset->loading, asset->loaded, false, true, asset->asset};
      return a;
    }
    else
    return Asset<T>();
  }
};