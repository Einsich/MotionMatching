#pragma once
#include <sys/types.h>
#include <unordered_map>
#include <assert.h>
#include "type_description.h"
#include <algorithm>
namespace ecs
{
  constexpr uint binPow = 8;
  constexpr uint binSize = 1u << binPow;
  constexpr uint binMask = binSize - 1;
  class ComponentContainer
  {
    std::vector<void*> data;
  public:
    string_hash typeHash, typeNameHash;
    uint count, capacity, sizeOf;
    Constructor constructor = nullptr;
    CopyConstructor copy_constructor = nullptr;
    Destructor destructor = nullptr;
    ComponentContainer();
    ComponentContainer(const TypeInfo &type_info, string_hash type_name_hash, int capacity);
    template<typename T>
    T* get_component(uint i)
    {
      if (0 <= i && i < count)
      {
        int j = i >> binPow;
        i &= binMask;
        return (T*)((char*)data[j] + i * sizeOf);
      }
      return nullptr;
    }
    template<typename T>
    const T* get_component(uint i) const
    {
      if (0 <= i && i < count)
      {
        int j = i >> binPow;
        i &= binMask;
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