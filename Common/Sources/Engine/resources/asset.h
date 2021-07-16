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

class AssetStub : IAsset
{
public:
  virtual void load(const filesystem::path &, bool) override{}
  virtual void free() override{}
  virtual bool edit() override{return false;}
};
template<typename T>
class Asset : public ISerializable
{
  static_assert(std::is_base_of<IAsset, T>::value);
  struct ResourceInfo
  {
    filesystem::path path;
    bool loading, loaded, edited;
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

  Asset(const filesystem::path &resource_path) :
  asset(new ResourceInfo{resource_path, false, false, false, T()})
  {  
    ifstream file(resource_path, ios::binary);
    if (!file.fail())
    {
      read(file, asset->asset);
    }
  }
  template<typename U>
  operator Asset<U>() const
  {
    return *((Asset<U>*)this);
  }
  T* operator->()
  {
    return &asset->asset;
  }
  const T* operator->() const 
  {
    if (!asset->loaded)
      asset->load();
    return &asset->asset;
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

  virtual size_t serialize(std::ostream& os) const override
  {
    return asset ? write(os, asset->path.string()) : 0;
  }
  virtual size_t deserialize(std::istream& is) override
  {
    string path;
    size_t size = read(is, path);
    *this = create_asset<T>(filesystem::path(path));
    return size;
  }
};