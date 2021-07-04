#pragma once
#include <unordered_map>
#include <vector>
#include "component_container.h"
namespace ecs
{
  struct TemplateInfo;
  class Archetype
  {
  public: 
    std::unordered_map<uint, ComponentContainer> components;
    int count, capacity;
    std::vector<FullTypeDescription*> fullTypeDescriptions;
    string synonim;
    Archetype(const vector<string_hash> &type_hashes, int count, const string &synonim);
    bool in_archetype(const vector<string_hash> &type_hashes) const;

    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    void add_entity(const ComponentInitializerList &list);
    void add_entity(const vector<const TemplateInfo*> &list);
    void destroy_entity(int index, bool without_copy = false);
    ~Archetype() = default;
    void copy(const Archetype *src);
    
  };
}