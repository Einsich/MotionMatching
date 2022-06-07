#pragma once
#include "compile_time_string.h"
#include "string_hash.h"
#include "component_function.h"
#include <unordered_map>
#include "../singleton.h"
#include "user_type_info.h"

#include <eastl/unique_ptr.h>
#include <eastl/vector_map.h>

namespace ecs
{

  template<class T>
  void* template_constructor(void *memory)
  {
    return new (memory) T();
  }
  template<class T>
  void template_destructor(void *memory)
  {
    return ((T*)(memory))->~T();
  }
  template<typename T>
  void* template_copy_constructor(const void *src, void *dst)
  {
    return new (dst) T(*((T*)src));
  }
  template<typename T>
  void* template_move_constructor(void *src, void *dst)
  {
    return new (dst) T(std::move(*((T*)src)));
  }
  struct TypeRTTI
  {
    const int sizeOf = 0;
    const Constructor constructor = nullptr;
    const CopyConstructor copy_constructor = nullptr;
    const MoveConstructor move_constructor = nullptr;
    const Destructor destructor = nullptr;
  };
  template<typename T>
  TypeRTTI get_rtti()
  {
    return TypeRTTI{ sizeof(T),
      ecs::template_constructor<T>,
      ecs::template_copy_constructor<T>,
      ecs::template_move_constructor<T>,
      ecs::template_destructor<T> };
  }
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

    TypeInfo(TypeRTTI rtti, std::string &&name, bool trivialCopy = false, bool trivialRelocatable = false, UserTypeInfo userInfo = UserTypeInfo()):
      hashId(HashedString(name)), name(std::move(name)),
      trivialCopy(trivialCopy), trivialRelocatable(trivialRelocatable),
      rtti(rtti),
      userInfo(userInfo)
    {
      debug_log("ECS: register %u type %s, sizeof %u, hash %u, trivial copy - %s, move - %s",
        TypeInfo::types().size(), this->name.c_str(), rtti.sizeOf, hashId,
        trivialCopy ? "true" : "false", trivialRelocatable ? "true" : "false");

      TypeInfo::types().try_emplace(hashId, this);
    }
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

  

}