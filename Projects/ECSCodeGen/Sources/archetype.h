#pragma once
#include <unordered_map>
#include <vector>
#include "component_container.h"
#include "type_description.h"
namespace ecs
{
  struct ComponentTypes
  {
    std::vector<TypeDescription> componentsTypes;
  };

  class Archetype
  {
    std::unordered_map<uint, ComponentContainer> components;
  public: 
    int componentCount;
    Archetype(const ComponentTypes &types, int count);
    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    ~Archetype() = default;
  };
}