#pragma once
#include <string>
#include "string_hash.h"
#include "core_interface.h"
namespace ecs
{
  struct FullTypeDescription
  {
    std::string name; 
    string_hash hash;
    uint sizeOf;
    void *(*constructor)(void*);
    FullTypeDescription(std::string &&name, string_hash hash, uint sizeOf, void *(*constructor)(void*));
    ~FullTypeDescription() = default;
  };
  struct TypeDescription
  {
    string_hash name_hash;
    uint typeId;
    TypeDescription(string_hash name_hash, uint typeId);
    template<typename T>
    TypeDescription(const char *name);
    string_hash hash() const;
  };

  template<class T>
  void* template_constructor(void *memory)
  {
    return new (memory) T();
  }
  template<typename T>
  TypeDescription get_type_description(const char *name)
  {
    uint globalTypeIndexCopy = global_type_index();
    TypeDescription type = {HashedString(name), type_index<T>()};
    if (global_type_index() != globalTypeIndexCopy)
    {
      full_description().emplace_back(std::move(std::string(name)), type.name_hash, sizeof(T), template_constructor<T>);
    }
    return type;
  }
}