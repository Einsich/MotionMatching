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
    static uint hash(string_hash name_hash, uint typeId)
    {
      return name_hash ^ (typeId * 16397816463u);
    }
    uint typeHash;
    const FullTypeDescription &fullDescription;
  public:
    TypeDescription(string_hash name_hash, uint typeId);
    TypeDescription(uint type_hash, const FullTypeDescription &full_description);

    const FullTypeDescription &get_full_description() const;

    template<typename T>
    static uint typeDescriptionHash(const char *name)
    {
      return hash(HashedString(name), type_index<T>());
    }

    uint type_hash() const;
    bool operator==(const TypeDescription &other);
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
    uint typeHash = TypeDescription::typeDescriptionHash<T>(name);
    auto it = full_description().find(typeHash);
    if (it == full_description().end())
    {
      auto result = full_description().try_emplace(typeHash, typeid(T).name(), name, typeHash, sizeof(T),
        template_copy_constructor<T>, template_destructor<T>);
      
      assert(result.second && "Failed to add new type in get_type_description");
      it = result.first;
    }
    return TypeDescription(typeHash, it->second);
  }
}