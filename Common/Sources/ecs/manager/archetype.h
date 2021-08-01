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
    std::string synonim;
    Archetype(const std::vector<string_hash> &type_hashes, int count, const std::string &synonim);
    bool in_archetype(const std::vector<string_hash> &type_hashes) const;

    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    void add_entity(const ComponentInitializerList &list);
    void add_entity(const std::vector<const TemplateInfo*> &list);
    void destroy_entity(int index, bool without_copy = false);
    ~Archetype() = default;
    void copy(const Archetype *src);
    
  };
}