#pragma once
#include <sys/types.h>
#include <unordered_map>
#include <assert.h>
#include "type_description.h"
#include <algorithm>
namespace ecs
{
  struct ComponentInitializer
  {
    void *data = nullptr;
    int sizeOf;
    Destructor descructor;

    template<typename T>
    void operator=(const T &value)
    {
      if (data == nullptr)
      {
        sizeOf = sizeof(T);
        data = malloc(sizeOf);
        descructor = template_destructor<T>;
      }
      else
      {
        assert(descructor == template_destructor<T> && "ComponentInitializer was initialized by other type, but hashes was equal!");
      }
      data = new (data) T(std::move(value));
    }
    ~ComponentInitializer()
    {
      if (data)
      {
        descructor(data);
        free(data);
      }
    }
  };
  struct ComponentInitializerList
  {
    std::unordered_map<uint, ComponentInitializer> components;
    ComponentTypes types;
    template<typename T>
    ComponentInitializer &add(const char *name)
    {
      
      TypeDescription type = get_type_description<T>(name);
      if (std::find(types.componentsTypes.begin(), types.componentsTypes.end(), type) == types.componentsTypes.end())
        types.componentsTypes.push_back(type);
      uint hash = type.type_name_hash();
      return components[hash];
    }

    template<typename T>
    T &get(const char *name)
    {
      return *((T*)(add<T>(name).data));
    }
  };
  constexpr int binSize = 500;
  class ComponentContainer
  {
    std::vector<void*> data;
  public:
    string_hash typeHash, typeNameHash;
    int count, capacity, sizeOf;
    ComponentContainer();
    ComponentContainer(string_hash type_hash, string_hash type_name_hash, int capacity, int sizeOf);
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
    void destroy_component(int i, bool without_copy);
    void add_component(void *component_data);
    void copy_components(const ComponentContainer &other);
    ~ComponentContainer();
  };
}