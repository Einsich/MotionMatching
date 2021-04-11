#pragma once
#include <unordered_map>
#include <vector>
#include "component_container.h"
namespace ecs
{

  class Archetype
  {
    std::unordered_map<uint, ComponentContainer> components;
  public: 
    int count, capacity;
    Archetype(const ComponentTypes &types, int count);
    bool in_archetype(const ComponentTypes &types);
    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    void add_entity(const ComponentInitializerList &list);
    void destroy_entity(int index, bool without_copy = false);
    ~Archetype() = default;
    
  };
}