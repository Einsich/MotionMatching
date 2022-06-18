#pragma once
#include <functional>
#include "type_info.h"
#include "type_description.h"
#include "string_hash.h"
#include <assert.h>
namespace ecs
{
  struct ComponentInstance 
  {
private:
    vector<char> instanceData;
  public:
    const TypeInfo *typeInfo;
    std::function<void(void*)> initManager;
    string name;
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

    template<typename T, typename TT = std::remove_cvref_t<T>>
    ComponentInstance(const string &name, const T &instance) : 
    instanceData(sizeof(TT)), typeInfo(&type_info<TT>()), name(name)
    , nameHash(HashedString(name)), typeHash(type_hash<TT>()), typeNameHash(TypeDescription::hash(nameHash, type_hash<TT>()))
    {
      typeInfo->rtti.copy_constructor(&instance, instanceData.data());
    }    
    template<typename T, typename TT = std::remove_cvref_t<T>>
    ComponentInstance(const string &name, T &&instance) : 
    instanceData(sizeof(TT)), typeInfo(&type_info<TT>()), name(name)
    , nameHash(HashedString(name)), typeHash(type_hash<TT>()), typeNameHash(TypeDescription::hash(nameHash, type_hash<TT>()))
    {
      typeInfo->rtti.move_constructor(&instance, instanceData.data());
    }
    ComponentInstance(const TypeInfo &info, const string &name, std::function<void(void*)> &&initManager) : 
    instanceData(), typeInfo(&info), initManager(initManager), name(name)
    , nameHash(HashedString(name)), typeHash(info.hashId), typeNameHash(TypeDescription::hash(nameHash, info.hashId))
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
  };
  using ComponentInitializerList = vector<ComponentInstance>;

  void patch_components(vector<ComponentInstance> &components, const vector<ComponentInstance> &patch);
  void patch_components(vector<ComponentInstance> &components, vector<ComponentInstance> &&patch);
}