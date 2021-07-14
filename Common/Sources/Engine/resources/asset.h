#pragma once
#include <future>
#include <type_traits>
#include <string>
#include <string_view>
#include <istream>
#include <fstream>
#include "ecs/manager/compile_time_string.h"
#include "Serialization/serialization.h"
class IAsset
{
  //called when this resource really needed - load data from disk 
  virtual void load(const std::string &path) = 0;
  //dispose resource - called on application end, or when we can unload resource
  virtual void free() = 0;
  //this fuction need to edit resource and call reload if it changed
  virtual bool edit() = 0;
};

/*
  virtual void create(std::istream &io) override;
  virtual void load(const std::string &path) override;
  virtual void free() override;
  virtual bool edit() override;
*/
class AssetStub : IAsset
{
public:
  virtual void load(const std::string &) override{}
  virtual void free() override{}
  virtual bool edit() override{return false;}
};
template<typename T>
class Asset
{
  static_assert(std::is_base_of<IAsset, T>::value);
  struct ResourceInfo
  {
    T asset;
    std::string path;
    bool loading, loaded;
    void load()
    {
      if (!loaded && !loading)
      {
        loading = true;
        iasset->load(path);
        loaded = true;
      }
    }
    void load_async()
    {
      if (!loaded && !loading)
      {
        loading = true;      
        std::async(std::launch::async, &IAsset::load, iasset, path);
      }
    }
    void save()
    {
      if (loaded)
        iasset->save(path);
    }
    void save_async()
    {
      if (loaded)
        std::async(std::launch::async, &IAsset::save, iasset, path);
    }
  };
  ResourceInfo *resource;
public:
  //create asset and init them from .meta file or only default value
  Asset(const std::string &resource_path) :
  resource(new ResourceInfo{T(), resource_path, false, false})
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
  void load(bool async = false)
  {
    if (async)
      resource->load_async();
    else
      resource->load();
  }
  void save()
  {
    ofstream file(resource_path, ios::binary);
    if (!file.fail())
    {
      write(file, resource->asset);
    }
  }
  constexpr std::string_view type() const
  {
    return nameOf<T>::value;
  }
};