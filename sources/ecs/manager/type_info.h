#pragma once
#include "component_function.h"
#include <unordered_map>
#include "../singleton.h"
#include "user_type_info.h"

#include <eastl/unique_ptr.h>
#include <eastl/vector_map.h>
namespace ecs
{
  struct TypeRTTI
  {
    const int sizeOf = 0;
    const Constructor constructor = nullptr;
    const CopyConstructor copy_constructor = nullptr;
    const MoveConstructor move_constructor = nullptr;
    const Destructor destructor = nullptr;
  };
  struct TypeInfo
  {
    static std::unordered_map<uint32_t, TypeInfo*>& types()
    {
      static std::unordered_map<uint32_t, TypeInfo*> types;
      return types;
    }
    
    const uint32_t hashId = 0;
    const std::string name = "";
    const bool trivialCopy = false;
    const bool trivialRelocatable = false;
    TypeRTTI rtti;
    const UserTypeInfo userInfo;

    TypeInfo(TypeRTTI rtti, std::string &&name, bool trivialCopy = false, bool trivialRelocatable = false, UserTypeInfo userInfo = UserTypeInfo());
    TypeInfo(const TypeInfo &) = delete;
    TypeInfo(TypeInfo &&) = delete;
    TypeInfo& operator= (const TypeInfo &) = delete;
    TypeInfo& operator= (TypeInfo &&) = delete;
  };
  struct SingletonTypeInfo : public TypeInfo
  {
    const GetSingleton getSingleton = nullptr;
    static std::unordered_map<uint32_t, SingletonTypeInfo>& types()
    {
      static std::unordered_map<uint32_t, SingletonTypeInfo> types;
      return types;
    }
  };

  template<typename T>
  void* template_singleton_instance()
  {
    return &get_singleton<T>();
  }

  template<typename T>
  uint32_t type_hash();

  template<typename T>
  const char* type_name();
}