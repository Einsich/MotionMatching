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
        auto destr = template_destructor<T>;
        assert(descructor == destr && "ComponentInitializer was initialized by other type, but hashes was equal!");
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
      uint hash = type.type_hash();
      return components[hash];
    }
    //template<typename T>
    ComponentInitializer &operator[](const char *name)
    {
      TypeDescription type(HashedString(name), type_index<int>());
      types.componentsTypes.push_back(type);
      uint hash = type.type_hash();
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
    uint typeHash;
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
    void destroy_component(int i, bool without_copy);
    void add_component(const ComponentInitializer &component);
    ~ComponentContainer();
  };
}