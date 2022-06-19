#pragma once
#include <eastl/vector_map.h>
#include <vector>
#include "component_container.h"
#include "component_instance.h"
namespace ecs
{
  struct TypeDescription
  {
    const char *name;
    const ecs::TypeInfo *typeInfo;
    const ComponentContainer *components;
    uint typeNameHash;
  };
  class Archetype
  {
  public: 
    int index;
    eastl::vector_map<uint, ComponentContainer> components;
    int count, capacity;
    std::vector<TypeDescription> typeDescriptions;
    std::string synonim;

    Archetype(int index, const std::vector<ComponentInstance> &type_hashes, int count, const std::string &synonim);

    bool in_archetype(const std::vector<ComponentInstance> &instances) const;

    ComponentContainer *get_container(uint type_name_hash);
    void destroy_entity(int index, bool with_swap);
    ~Archetype() = default;
    void copy(const Archetype *src);
    
  };
}