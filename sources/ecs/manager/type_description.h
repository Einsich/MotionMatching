#pragma once
#include <string>
#include <vector>
#include "type_info.h"
#include "core_interface.h"
#include "string_hash.h"

namespace ecs
{
  struct FullTypeDescription
  {
    std::string name; 
    uint typeHash, hash;
    FullTypeDescription(const char *name, uint typeHash, uint hash);
    FullTypeDescription(){}
    const TypeInfo &get_type_info() const;
  };
  struct TypeDescription
  {
  private:
    uint typeNameHash;
  public:
  
    constexpr static uint hash(uint name_hash, uint type_hash)
    {
      return name_hash ^ (type_hash * 16397816463u);
    }
    TypeDescription(uint name_hash, uint type_hash);
    TypeDescription(uint type_hash);

    const FullTypeDescription &get_full_description() const;

    template<typename T>
    static uint typeDescriptionHash(const char *name)
    {
      return hash(HashedString(name), ecs::type_hash<T>());
    }

    uint type_name_hash() const;
    bool operator==(const TypeDescription &other);
  };

  template<typename T>
  uint register_type_description(const char *name)
  {
    if constexpr (is_singleton<T>::value)
    {
      return 0;
    }
    else
    {
      uint typeHash = type_hash<T>();
      uint typeNameHash = TypeDescription::hash(HashedString(name), typeHash);
      auto it = full_description().find(typeNameHash);
      if (it == full_description().end())
      {
        auto result = full_description().try_emplace(typeNameHash, name, typeHash, typeNameHash);
        assert(result.second && "Failed to add new type in get_type_description");
        it = result.first;
      }
      return typeNameHash;
    }
  }
  template<typename T>
  constexpr uint get_type_description(const char *name)
  {
    if constexpr (is_singleton<T>::value)
    {
      return 0;
    }
    else
    {
      return TypeDescription::hash(HashedString(name), ecs::type_hash<T>());
    }
  }
}