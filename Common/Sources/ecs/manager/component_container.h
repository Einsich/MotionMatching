#pragma once
#include <sys/types.h>
#include <unordered_map>
#include <assert.h>
#include "type_description.h"
namespace ecs
{
  struct ComponentInitializer
  {
    void *data = nullptr;
    int sizeOf;
    template<typename T>
    void operator=(const T &value)
    {
      if (data == nullptr)
      {
        sizeOf = sizeof(T);
        data = malloc(sizeOf);
      }
      else
      {
        assert(sizeOf == sizeof(T) && "ComponentInitializer was initialized by other type, but hashes was equal!");
      }
      data = new (data) T(std::move(value));
    }
    ~ComponentInitializer()
    {
      free(data);
    }
  };
  struct ComponentInitializerList
  {
    std::unordered_map<uint, ComponentInitializer> components;
    ComponentTypes types;
    template<typename T>
    ComponentInitializer &add(const char *name)
    {
      TypeDescription type(HashedString(name), type_index<T>());
      types.componentsTypes.push_back(type);
      uint hash = type.hash();
      return components[hash];
    }
    template<typename T>
    ComponentInitializer &get(const char *name)
    {
      return add<T>(name);
    }
  };
  constexpr int binSize = 500;
  class ComponentContainer
  {
    std::vector<void*> data;
  public:
    uint typeID;
    int count, capacity;
    ComponentContainer();
    ComponentContainer(uint type, int capacity);
    template<typename T>
    T* get_component(int i)
    {
      if (0 <= i && i < count)
      {
        int j = i / binSize;
        i %= binSize;
        return ((T*)data[j] + i);
      }
      return nullptr;
    }
    void destroy_component(int i);
    void add_component(const ComponentInitializer &component);
    ~ComponentContainer();
  };
}