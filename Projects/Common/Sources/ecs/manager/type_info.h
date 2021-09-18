#pragma once
#include "compile_time_string.h"
#include "string_hash.h"
#include "component_function.h"
#include <unordered_map>
#include "../singleton.h"
#include "../component_editor.h"

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
    const Serializer serialiser = nullptr;
    const Deserializer deserialiser = nullptr;
  
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
  struct RegisterTypeInfoRT
  {
    
  };

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
  bool template_component_edition(void *ptr, bool view_only)
  {
    return edit_component(*((T*)ptr), nameOf<T>::value.data(), view_only);
  }
  
  template<typename T>
  size_t template_serializer(std::ostream& os, const void *ptr)
  {
    return write(os, *((T*)ptr));
  }

  template<typename T>
  size_t template_deserializer(std::istream& is, void *ptr)
  {
    return read(is, *((T*)ptr));
  }
  template<typename T>
  void* template_singleton_instance()
  {
    return &get_singleton<T>();
  }
  template<typename T>
  RegisterTypeInfoRT register_type()
  {
    constexpr string_hash hash = HashedString(nameOf<T>::value);
    
    if constexpr (is_singleton<T>::value)
    {
      SingletonTypeInfo::types().try_emplace(
        hash, SingletonTypeInfo{{hash, string(nameOf<T>::value), sizeof(T), 
        ecs::template_constructor<T>, ecs::template_copy_constructor<T>, ecs::template_destructor<T>,
        ecs::template_component_edition<T>, ecs::template_serializer<T>, ecs::template_deserializer<T>},
        template_singleton_instance<T>});
    }
    else
    {
      TypeInfo::types().try_emplace(
        hash, TypeInfo{hash, string(nameOf<T>::value), sizeof(T), 
        ecs::template_constructor<T>, ecs::template_copy_constructor<T>, ecs::template_destructor<T>,
        ecs::template_component_edition<T>, ecs::template_serializer<T>, ecs::template_deserializer<T>});
    }
    return RegisterTypeInfoRT();
  }
  #define REG_TYPE_BASE(T, NAME) \
  static ecs::RegisterTypeInfoRT  NAME ##_register = ecs::register_type<T>();
  #define REG_TYPE_BASE(T, NAME) \
  static ecs::RegisterTypeInfoRT  NAME ##_register = ecs::register_type<T>();
  
  #define REG_TYPE(T) REG_TYPE_BASE(T, type_##T)
  #define REG_VEC_TYPE(T) REG_TYPE_BASE(std::vector<T>, vec_##T)

  #define REGISTER_TYPE(T) REG_TYPE(T) REG_VEC_TYPE(T) 

  #define REG_TYPE_NAMED(T, name) REG_TYPE_BASE(T, name)
  #define REG_VEC_TYPE_NAMED(T, name) REG_TYPE_BASE(std::vector<T>, name)

  #define REGISTER_TYPE_NAMED(T, name) REG_TYPE_NAMED(T, name) REG_VEC_TYPE_NAMED(T, vec##name) 
}