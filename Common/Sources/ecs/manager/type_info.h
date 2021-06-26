#pragma once
#include "string_hash.h"
#include "Serialization/reflection.h"
#include "component_function.h"
#include <unordered_map>
namespace ecs
{

  struct TypeInfo//run-time
  {
    static std::unordered_map<uint32_t, TypeInfo>& types()
    {
      static std::unordered_map<uint32_t, TypeInfo> types;
      return types;
    }
    const uint32_t hashId = 0;
    const std::string name = "";
    const int sizeOf = 0;
    const Constructor constructor = nullptr;
    const CopyConstructor copy_constructor = nullptr;
    const Destructor destructor = nullptr;
    const ComponentEdition componentEdition = nullptr;

    TypeInfo(const uint32_t hashId, 
    const std::string_view &name, 
    const int sizeOf,
    const Constructor constructor,
    const CopyConstructor copy_constructor,
    const Destructor destructor,
    const ComponentEdition componentEdition):
    hashId(hashId), name(name), sizeOf(sizeOf), 
    constructor(constructor),
    copy_constructor(copy_constructor),
    destructor(destructor), componentEdition(componentEdition)
    {
    }
    TypeInfo(){}
  };
  struct RegisterTypeInfoRT
  {
    
  };
  template<typename... Args>
  RegisterTypeInfoRT register_type(const std::string_view &name, Args &&...args)
  {
    string_hash hash = HashedString(name);
    TypeInfo::types().try_emplace(hash, hash, name, args...);
    return RegisterTypeInfoRT();
  }
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
  void* template_copy_constructor(void *src, void *dst)
  {
    return new (dst) T(std::move(*((T*)src)));
  }
  template<typename T>
  std::enable_if_t<HasReflection<T>::value, void> template_component_edition(void *ptr)
  {
    ((T*)ptr)->editor_reflect();
  }
  template<typename T>
  std::enable_if_t<!HasReflection<T>::value, void> template_component_edition(void *ptr)
  {
    edit_component(*((T*)ptr), nameOf<T>::value.data());
  }
  #define REG_TYPE_BASE(T, NAME) \
  static ecs::RegisterTypeInfoRT  NAME ##_register = ecs::register_type(nameOf<T>::value, sizeof(T), ecs::template_constructor<T>, ecs::template_copy_constructor<T>, ecs::template_destructor<T>, ecs::template_component_edition<T>);
  
  #define REG_TYPE(T) REG_TYPE_BASE(T, type_##T)
  #define REG_VEC_TYPE(T) REG_TYPE_BASE(std::vector<T>, vec_##T)

  #define REGISTER_TYPE(T) REG_TYPE(T) REG_VEC_TYPE(T) 
}