#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "type_info.h"
#include "core_interface.h"
#include "../component_editor.h"
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
      constexpr std::string_view str = nameOf<T>::value;
      return hash(HashedString(name), HashedString(str));
    }

    uint type_name_hash() const;
    bool operator==(const TypeDescription &other);
  };
  struct ComponentTypes
  {
    std::vector<TypeDescription> componentsTypes;
  };



  template<typename T>
  TypeDescription get_type_description(const char *name)
  {
    constexpr std::string_view type = nameOf<T>::value;
    static_assert(HashedString(type), "need to register this type to ecs, use REG_TYPE(T)");
    
    constexpr string_hash typeHash = HashedString(type);
    string_hash typeNameHash = TypeDescription::hash(HashedString(name), typeHash);
    auto it = full_description().find(typeNameHash);
    if (it == full_description().end())
    {
      auto result = full_description().try_emplace(typeNameHash, name, typeHash, typeNameHash);
      assert(result.second && "Failed to add new type in get_type_description");
      it = result.first;
    }
    return TypeDescription(typeNameHash);
  }
}