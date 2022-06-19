#pragma once
#include <functional>
#include "type_info.h"
#include "string_hash.h"
#include <assert.h>
namespace ecs
{
  struct ComponentInstance 
  {
private:
    std::vector<char> instanceData;
  public:
    const TypeInfo *typeInfo;
    std::function<void(void*)> initManager;
    std::string name;
    uint nameHash, typeHash, typeNameHash;
    ComponentInstance(ComponentInstance &&) = default;
    ComponentInstance& operator=(ComponentInstance &&) = default;
    
    ComponentInstance(const ComponentInstance &other):
    instanceData(), typeInfo(other.typeInfo), initManager(other.initManager), name(other.name),
    nameHash(other.nameHash), typeHash(other.typeHash), typeNameHash(other.typeNameHash)
    {
      if (!initManager)
      {
        instanceData.resize(typeInfo->rtti.sizeOf);
        typeInfo->rtti.copy_constructor(other.get_data(), get_data());
      }      
    }
    ComponentInstance& operator=(const ComponentInstance &other)
    {
      assert(typeHash == other.typeHash);
      if (!instanceData.empty())
        typeInfo->rtti.destructor(get_data());
      if (other.initManager)
      {
        initManager = other.initManager;
        instanceData.clear();
      }
      else
      {
        instanceData.resize(typeInfo->rtti.sizeOf);
        typeInfo->rtti.copy_constructor(other.get_data(), get_data());
      }
      return *this;
    }

    template<typename T>
    ComponentInstance(const std::string &name, T &instance) : 
    instanceData(sizeof(T)), typeInfo(&type_info<T>()), name(name)
    , nameHash(HashedString(name)), typeHash(type_hash<T>()), typeNameHash(type_name_hash(nameHash, type_hash<T>()))
    {
      typeInfo->rtti.copy_constructor(&instance, instanceData.data());
    }    
    template<typename T>
    ComponentInstance(const std::string &name, const T &instance) : 
    instanceData(sizeof(T)), typeInfo(&type_info<T>()), name(name)
    , nameHash(HashedString(name)), typeHash(type_hash<T>()), typeNameHash(type_name_hash(nameHash, type_hash<T>()))
    {
      typeInfo->rtti.copy_constructor(&instance, instanceData.data());
    }    
    template<typename T>
    ComponentInstance(const std::string &name, T &&instance) : 
    instanceData(sizeof(T)), typeInfo(&type_info<T>()), name(name)
    , nameHash(HashedString(name)), typeHash(type_hash<T>()), typeNameHash(type_name_hash(nameHash, type_hash<T>()))
    {
      typeInfo->rtti.move_constructor(&instance, instanceData.data());
    }
    ComponentInstance(const TypeInfo &info, const std::string &name, std::function<void(void*)> &&initManager) : 
    instanceData(), typeInfo(&info), initManager(initManager), name(name)
    , nameHash(HashedString(name)), typeHash(info.hashId), typeNameHash(type_name_hash(nameHash, info.hashId))
    {
    }
   
    void *get_data()
    {
      assert(!instanceData.empty());
      return instanceData.data();
    }   
    const void *get_data() const 
    {
      assert(!instanceData.empty());
      return instanceData.data();
    }  
    void init_from_func()
    {
      assert(instanceData.empty());
      instanceData.resize(typeInfo->rtti.sizeOf);
      initManager(instanceData.data());
      initManager = nullptr;
    }  
  };
  using ComponentInitializerList = std::vector<ComponentInstance>;

  void patch_components(std::vector<ComponentInstance> &components, const std::vector<ComponentInstance> &patch);
  void patch_components(std::vector<ComponentInstance> &components, std::vector<ComponentInstance> &&patch);
}