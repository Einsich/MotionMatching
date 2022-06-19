#pragma once
#include "component_container.h"
#include "component_instance.h"
namespace ecs
{
  struct TypeDescription
  {
    ecs::string name;
    const ecs::TypeInfo *typeInfo;
    const ComponentContainer *components;
    uint typeNameHash;
  };
  class Archetype
  {
  public: 
    int index;
    ecs::map<uint, ComponentContainer> components;
    int count, capacity;
    ecs::vector<TypeDescription> typeDescriptions;
    ecs::string synonim;

    Archetype(int index, const ecs::vector<ComponentInstance> &type_hashes, int count, const ecs::string &synonim);

    bool in_archetype(const ecs::vector<ComponentInstance> &instances) const;

    ComponentContainer *get_container(uint type_name_hash);
    void destroy_entity(int index, bool with_swap);
    ~Archetype() = default;
    void copy(const Archetype *src);
    
  };
}