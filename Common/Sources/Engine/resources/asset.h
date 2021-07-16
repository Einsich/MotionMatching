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
  virtual void load(const filesystem::path &path) override;
  virtual void free() override;
  virtual bool edit() override;
*/
class AssetStub : IAsset
{
public:
  virtual void load(const filesystem::path &, bool) override{}
  virtual void free() override{}
  virtual bool edit() override{return false;}
};
template<typename T>
class Asset
{
  static_assert(std::is_base_of<IAsset, T>::value);
  struct ResourceInfo
  {
    filesystem::path path;
    bool loading, loaded;
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
  ResourceInfo *resource;
public:
  ResourceInfo *get_resource() const
  {
    return resource;
  }
  //create asset and init them from .meta file or only default value
  Asset():resource(nullptr){}

  Asset(const filesystem::path &resource_path) :
  resource(new ResourceInfo{resource_path, false, false, T()})
  {  
    ifstream file(resource_path, ios::binary);
    if (!file.fail())
    {
      read(file, resource->asset);
    }
  }

  T* operator->()
  {
    return &resource->asset;
  }
  const T* operator->() const 
  {
    if (!resource->loaded)
      resource->load();
    return &resource->asset;
  }
  void load(bool async = false)
  {
    if (async)
      resource->load_async();
    else
      resource->load();
  }
  void reload()
  {
    resource->reload();
  }
  void save() const
  {
    ofstream file(resource->path, ios::binary);
    if (!file.fail())
    {
      write(file, resource->asset);
    }
  }
  bool edit()
  {
    return resource->asset.edit();
  }
  bool loaded() const
  {
    return resource->loaded;
  }
};