#pragma once
#include "config/config.h"
#include "../singleton.h"
#include "config/user_type_info.h"
#include "string_hash.h"
namespace ecs
{
  typedef void (*Constructor)(void*);
  typedef void (*CopyConstructor)(const void*, void*);
  typedef void (*MoveConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
  struct TypeRTTI
  {
    const int sizeOf = 0;
    const Constructor constructorFunc = nullptr;
    const CopyConstructor copy_constructorFunc = nullptr;
    const MoveConstructor move_constructorFunc = nullptr;
    const Destructor destructorFunc = nullptr;
    const bool trivialCopy = false;
    const bool trivialMove = false;
    const bool trivialConstruct = false;
    const bool trivialDestruct = false;
    inline void constructor(void *memory) const
    {
      if (trivialConstruct)
        memset(memory, 0, sizeOf);
      else
        constructorFunc(memory);
    }
    inline void copy_constructor(const void*src, void*dst) const
    {
      if (trivialCopy)
        memcpy(dst, src, sizeOf);
      else
        copy_constructorFunc(src, dst);
    }
    inline void move_constructor(void*src, void*dst) const
    {
      if (trivialCopy)
      {
        memcpy(dst, src, sizeOf);
        memset(src, 0, sizeOf);
      }
      else
        move_constructorFunc(src, dst);
    }
    inline void destructor(void *memory) const
    {
      if (!trivialDestruct)
        destructorFunc(memory);
    }
  };
  struct TypeInfo
  {
    static ecs::map<uint32_t, TypeInfo*>& types()
    {
      static ecs::map<uint32_t, TypeInfo*> types;
      return types;
    }

    const uint32_t hashId;
    const ecs::string name;
    const TypeRTTI rtti;
    const UserTypeInfo userInfo;

    TypeInfo(TypeRTTI rtti, ecs::string &&name, UserTypeInfo userInfo = UserTypeInfo());
    TypeInfo(const TypeInfo &) = delete;
    TypeInfo(TypeInfo &&) = delete;
    TypeInfo& operator= (const TypeInfo &) = delete;
    TypeInfo& operator= (TypeInfo &&) = delete;
  };
  struct SingletonTypeInfo : public TypeInfo
  {
    const GetSingleton getSingleton = nullptr;
    static ecs::map<uint32_t, SingletonTypeInfo>& types()
    {
      static ecs::map<uint32_t, SingletonTypeInfo> types;
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

  template<typename T>
  const TypeInfo &type_info();

  constexpr inline uint type_name_hash(uint name_hash, uint type_hash)
  {
    return name_hash ^ (type_hash * 16397816463u);
  }

  template<typename T>
  constexpr uint get_type_hash()
  {
    if constexpr (is_singleton<T>::value)
      return 0;
    else
      return ecs::type_hash<T>();
  }
  
  constexpr uint get_name_hash(const char *name)
  {
    return HashedString(name);
  }
}