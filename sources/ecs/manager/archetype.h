#pragma once
#include <eastl/vector_map.h>
#include <vector>
#include "component_container.h"
#include "component_instance.h"
namespace ecs
{
  class Archetype
  {
  public: 
    int index;
    eastl::vector_map<uint, ComponentContainer> components;
    int count, capacity;
    std::vector<FullTypeDescription*> fullTypeDescriptions;
    string synonim;
    bool dontSave;

    //after this constructor need to feel components and fullTypeDescriptions correctly
    Archetype(int index, int count, const string &synonim, int type_count);
    Archetype(int index, const vector<uint> &type_hashes, int count, const string &synonim);
    bool in_archetype(const vector<uint> &type_hashes) const;

    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    vector<void*> get_entity_data(const vector<uint> &type_hashes);
    void destroy_entity(int index, bool with_swap);
    ~Archetype() = default;
    void copy(const Archetype *src);
    
  };
}