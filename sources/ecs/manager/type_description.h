#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "type_info.h"
#include "core_interface.h"

namespace ecs
{
  struct FullTypeDescription
  {
    std::string name; 
    string_hash typeHash, hash;
    FullTypeDescription(const char *name, string_hash typeHash, string_hash hash);
    FullTypeDescription(){}
    const TypeInfo &get_type_info() const;
  };
  struct TypeDescription
  {
  private:
    string_hash typeNameHash;
  public:
  
    constexpr static string_hash hash(string_hash name_hash, string_hash type_hash)
    {
      return name_hash ^ (type_hash * 16397816463u);
    }
    TypeDescription(string_hash name_hash, string_hash type_hash);
    TypeDescription(string_hash type_hash);

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
  string_hash register_type_description(const char *name)
  {
    if constexpr (is_singleton<T>::value)
    {
      return 0;
    }
    else
    {
      string_hash typeHash = type_hash<T>();
      string_hash typeNameHash = TypeDescription::hash(HashedString(name), typeHash);
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
  constexpr string_hash get_type_description(const char *name)
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