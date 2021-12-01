#pragma once
#include <string>
#include <vector>
#include <array>
#include <istream>
#include <ostream>
#include <memory>
#include "type_tuple.h"
#include "3dmath.h"


class DataBlock
{
public:
  using AllowedTypes = TypeTuple<
      int, ivec2, ivec3, ivec4,
      unsigned, uvec2, uvec3, uvec4,
      float, vec2, vec3, vec4,
      std::string, bool>;
      
  struct Property
  {
    std::string name;
    int type;
    int index;
    Property(const char*name, int type, int index):
      name(name), type(type), index(index) {}
  };
private:
  template<typename U, typename ...Ts> struct belong_to
  {
    static constexpr bool value = (std::is_same<U, Ts>::value || ... );
  };
  template<typename T>
  struct PropertiesTuple ;
  template<template <typename...> class Tuple, typename ...Args>
  struct PropertiesTuple<Tuple<Args...>> 
  {
    std::tuple<std::vector<Args>...> fields;
    std::array<std::vector<int>, sizeof...(Args)> indices;
  };
  std::string blockName, blockType;
  PropertiesTuple<AllowedTypes> typeList;
  std::vector<Property> properties;
  std::vector<std::shared_ptr<DataBlock>> blocks;

  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  bool find_implementation(const char *name, uint32_t &index) const 
  {
    const std::vector<int> &indicesWithT = std::get<N>(typeList.indices);
    for (uint32_t i = 0, n = indicesWithT.size(); i < n; i++)
    {
      if (strcmp(properties[indicesWithT[i]].name.c_str(), name) == 0)
      {
        index = i;
        return true;
      }
    }
    return false;
  }
  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  const T* get_implementation(const char *name) const
  {
    uint32_t index;
    if (find_implementation<T>(name, index))
    {
      return &std::get<N>(typeList.fields)[index];
    }
    return nullptr;
  }
  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  T* get_implementation(const char *name)
  {
    uint32_t index;
    if (find_implementation<T>(name, index))
    {
      return &std::get<N>(typeList.fields)[index];
    }
    return nullptr;
  }
  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  void add_implementation(const char *name, const T &value)
  {
    std::vector<T> &fieldsWithT = std::get<N>(typeList.fields);
    std::vector<int> &indicesWithT = std::get<N>(typeList.indices);
    int index = fieldsWithT.size();
    fieldsWithT.emplace_back(value);
    indicesWithT.emplace_back(properties.size());
    properties.emplace_back(name, N, index);
  }
  

public:
  DataBlock() = default;
  DataBlock(const char *name, const char *type);
  DataBlock(const std::string &path);
  DataBlock(const char *path);
  DataBlock(std::ifstream &stream);

  const std::string& name() const;
  const std::string& type() const;
  DataBlock *getBlock(const char *name);
  const DataBlock *getBlock(const char *name) const;
  DataBlock *getBlock(size_t index);
  const DataBlock *getBlock(size_t index) const;

  const Property &getProperty(size_t index) const;

  DataBlock* addBlock(const char *name, const char *type = nullptr);

  size_t blockCount() const;
  size_t propertiesCount() const;
  
  template<typename T>
  const T& get(const char* name, const T &default_value) const
  {
    const T* value = get_implementation<T>(name);
    return value ? *value : default_value;
  }
  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  std::enable_if_t<!(sizeof(T) <= sizeof(size_t)), const T&> get(const Property &property) const
  {
    assert(property.type == N);
    return std::get<N>(typeList.fields)[property.index];
  }
  template<typename T, size_t N = AllowedTypes::getIndexOfType<T>()>
  std::enable_if_t<sizeof(T) <= sizeof(size_t), T> get(const Property &property) const
  {
    assert(property.type == N);
    return std::get<N>(typeList.fields)[property.index];
  }
  
  template<typename T>
  T* get(const char* name)
  {
    return get_implementation<T>(name);
  }
  template<typename T>
  const T* get(const char* name) const 
  {
    return get_implementation<T>(name);
  }

  template<typename T>
  void add(const char* name, const T &default_value)
  {
    add_implementation<T>(name, default_value);
  }

  template<typename T>
  bool exists(const char* name)
  {
    uint32_t index;
    return find_implementation<T>(name, index);
  }
};
using DataBlockPtr = std::shared_ptr<DataBlock>;

#define GET_FUNCTIONS(array_name, function_name)\
template<std::size_t... Is, std::size_t N = sizeof...(Is)>\
constexpr auto get_functions##function_name(std::index_sequence<Is...>)\
{\
  return std::array<decltype(&function_name<DataBlock::AllowedTypes::TypeOf<0>>), N> {\
      (&function_name<DataBlock::AllowedTypes::TypeOf<Is>>)...};\
}\
static constexpr array<decltype(&function_name<DataBlock::AllowedTypes::TypeOf<0>>), DataBlock::AllowedTypes::size>\
  array_name = get_functions##function_name(std::make_index_sequence<DataBlock::AllowedTypes::size>());

