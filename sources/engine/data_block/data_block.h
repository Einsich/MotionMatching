#pragma once
#include <string>
#include <vector>
#include <array>
#include <istream>
#include <ostream>
#include "3dmath.h"

class DataBlock
{
  template<typename U, typename ...Ts> struct belong_to
  {
    static constexpr bool value = (std::is_same<U, Ts>::value || ... );
  };
  template<typename ...Args>
  struct TypeList
  {
    std::tuple<std::vector<Args>...> fields;
    std::array<std::vector<int>, sizeof...(Args)> indices;
    template<typename T, typename H, typename ... B>
    static constexpr size_t getTypeIndexInTemplateList()
    {
      if constexpr (std::is_same<T, H>::value)
        return 0;
      else
        return 1 + getTypeIndexInTemplateList<T, B...>();
    }
    template<typename T>
    static constexpr size_t getIndexOfType() {  return getTypeIndexInTemplateList<T, Args...>(); }
  };
  using MyTypeList = TypeList<
      int, ivec2, ivec3, ivec4,
      unsigned, uvec2, uvec3, uvec4,
      float, vec2, vec3, vec4,
      std::string, bool>;
  enum VarType
  {
    Int, String, Float
  };
  struct Field
  {
    std::string name;
    int type;
    int index;
    Field(const char*name, int type, int index):
      name(name), type(type), index(index) {}
  };
  std::string blockName, blockType;
  MyTypeList typeList;
  std::vector<Field> fields;
  std::vector<std::shared_ptr<DataBlock>> blocks;

  template<typename T, size_t N = MyTypeList::getIndexOfType<T>()>
  bool find_implementation(const char *name, uint32_t &index) const 
  {
    const std::vector<int> &indicesWithT = std::get<N>(typeList.indices);
    for (uint32_t i = 0, n = indicesWithT.size(); i < n; i++)
    {
      if (strcmp(fields[indicesWithT[i]].name.c_str(), name) == 0)
      {
        index = i;
        return true;
      }
    }
    return false;
  }
  template<typename T, size_t N = MyTypeList::getIndexOfType<T>()>
  const T* get_implementation(const char *name) const
  {
    uint32_t index;
    if (find_implementation<T>(name, index))
    {
      return &std::get<N>(typeList.fields)[index];
    }
    return nullptr;
  }
  template<typename T, size_t N = MyTypeList::getIndexOfType<T>()>
  T* get_implementation(const char *name)
  {
    uint32_t index;
    if (find_implementation<T>(name, index))
    {
      return &std::get<N>(typeList.fields)[index];
    }
    return nullptr;
  }
  template<typename T, size_t N = MyTypeList::getIndexOfType<T>()>
  void add_implementation(const char *name, const T &value)
  {
    std::vector<T> &fieldsWithT = std::get<N>(typeList.fields);
    std::vector<int> &indicesWithT = std::get<N>(typeList.indices);
    int index = fieldsWithT.size();
    fieldsWithT.emplace_back(value);
    indicesWithT.emplace_back(fields.size());
    fields.emplace_back(name, N, index);
  }
  

public:
  DataBlock() = default;
  DataBlock(const char *name, const char *type);
  DataBlock(const std::string &path);
  DataBlock(const char *path);
  DataBlock(std::ifstream &stream);

  const char* name() const;
  DataBlock *getBlock(const char *name);
  const DataBlock *getBlock(const char *name) const;
  DataBlock* addBlock(const char *name, const char *type = nullptr);

  
  template<typename T>
  const T& get(const char* name, const T &default_value) const
  {
    const T* value = get_implementation<T>(name);
    return value ? *value : default_value;
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

