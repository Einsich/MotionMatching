#pragma once
#include <string>
#include <vector>
#include "string_hash.h"
#include "core_interface.h"
namespace ecs
{
  struct FullTypeDescription
  {
    std::string name; 
    string_hash hash;
    uint sizeOf;
    Constructor constructor;
    CopyConstructor copy_constructor;
    Destructor destructor;
    FullTypeDescription(std::string &&name, string_hash hash, uint sizeOf,
      Constructor constructor, CopyConstructor copy_constructor, Destructor destructor);
    ~FullTypeDescription() = default;
  };
  struct TypeDescription
  {
    string_hash name_hash;
    uint typeId;
    TypeDescription(string_hash name_hash, uint typeId);

    template<typename T>
    TypeDescription(const char *name):
      name_hash(HashedString(name)), typeId(type_index<T>()){}

    
    string_hash hash() const;
  };
  struct ComponentTypes
  {
    std::vector<TypeDescription> componentsTypes;
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
  void* template_copy_constructor(void *src, void *dst)
  {
    return new (dst) T(std::move(*((T*)src)));
  }
  template<typename T>
  TypeDescription get_type_description(const char *name)
  {
    uint globalTypeIndexCopy = global_type_index();
    TypeDescription type = {HashedString(name), type_index<T>()};
    if (global_type_index() != globalTypeIndexCopy)
    {
      full_description().emplace_back(std::move(std::string(name)), type.name_hash, sizeof(T),
        template_constructor<T>, template_copy_constructor<T>, template_destructor<T>);
    }
    return type;
  }
}