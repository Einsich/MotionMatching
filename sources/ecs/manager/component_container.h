#pragma once
#include <sys/types.h>
#include <unordered_map>
#include <assert.h>
#include "type_description.h"
#include <algorithm>
namespace ecs
{
  constexpr int binSize = 8;
  class ComponentContainer
  {
    std::vector<void*> data;
  public:
    string_hash typeHash, typeNameHash;
    int count, capacity, sizeOf;
    Constructor constructor = nullptr;
    CopyConstructor copy_constructor = nullptr;
    Destructor destructor = nullptr;
    ComponentContainer();
    ComponentContainer(const TypeInfo &type_info, string_hash type_name_hash, int capacity);
    template<typename T>
    T* get_component(int i)
    {
      if (0 <= i && i < count)
      {
        int j = i / binSize;
        i %= binSize;
        return (T*)((char*)data[j] + i * sizeOf);
      }
      return nullptr;
    }
    template<typename T>
    const T* get_component(int i) const
    {
      if (0 <= i && i < count)
      {
        int j = i / binSize;
        i %= binSize;
        return (T*)((char*)data[j] + i * sizeOf);
      }
      return nullptr;
    }
    template<typename T>
    T* get_component(uint binIndex, uint inBinIndex)
    {
      return (binIndex < data.size() && inBinIndex < binSize) ? (T*)data[binIndex] + inBinIndex : nullptr;
    }
    
    template<typename T>
    const T* get_component(uint binIndex, uint inBinIndex) const
    {
      return (binIndex < data.size() && inBinIndex < binSize) ? (T*)data[binIndex] + inBinIndex : nullptr;
    }
    void destroy_component(int i, bool without_copy);
    void add_component(void *component_data);
    void* add_component();
    void copy_components(const ComponentContainer &other);
    ~ComponentContainer();
  };
}