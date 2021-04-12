#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "string_hash.h"
#include "core_interface.h"

namespace ecs
{
  struct FullTypeDescription
  {
    std::string type, name; 
    string_hash hash;
    uint sizeOf;
    CopyConstructor copy_constructor;
    Destructor destructor;
    FullTypeDescription();
    FullTypeDescription(const char *type, const char *name, string_hash hash, uint sizeOf,
      CopyConstructor copy_constructor, Destructor destructor);
    ~FullTypeDescription() = default;
  };
  struct TypeDescription
  {
  private:
    static int hash(string_hash name_hash, uint typeId)
    {
      return name_hash ^ (typeId * 16397816463u);
    }
    uint typeHash;
  public:
    TypeDescription(string_hash name_hash, uint typeId);

    template<typename T>
    TypeDescription(const char *name):
      typeHash(hash(HashedString(name), type_index<T>())){}

    uint type_hash() const;
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
    TypeDescription type(HashedString(name), type_index<T>());
    auto it = full_description().find(type.type_hash());
    if (it == full_description().end())
    {
      full_description().try_emplace(type.type_hash(), typeid(T).name(), name, type.type_hash(), sizeof(T),
        template_copy_constructor<T>, template_destructor<T>);
    }
    return type;
  }
}