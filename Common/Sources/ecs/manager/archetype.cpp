#include "archetype.h"
#include "../editor/template.h"
namespace ecs
{
  static ComponentContainer *dummyContainer = new ComponentContainer();

  Archetype::Archetype(const vector<string_hash> &type_hashes, int capacity, const string &synonim):
    components(), count(0), capacity(capacity), synonim(synonim)
  {
    for(string_hash typeNameHash : type_hashes)
    {
      auto it = full_description().find(typeNameHash);
      assert(it->first && "Don't found full descr for type in Archetype");
      auto typeIt = TypeInfo::types().find(it->second.typeHash);
      assert(typeIt->first && "Don't found this type");
      fullTypeDescriptions.push_back(&it->second);
      components.try_emplace(typeNameHash, typeIt->second.hashId, typeNameHash, capacity, typeIt->second.sizeOf);
    }    
  }

  bool Archetype::in_archetype(const vector<string_hash> &type_hashes) const
  {
    if (type_hashes.size() != components.size())
      return false;
    for (string_hash descr : type_hashes)
    {
      auto it = components.find(descr);
      if (it == components.end())
        return false;
    }
    return true;
  }
  ComponentContainer *Archetype::get_container(const TypeDescription &type)
  {
    auto it = components.find(type.type_name_hash
    ());
    return it == components.end() ? dummyContainer : &it->second;
  }
  template<typename T>
  ComponentContainer *Archetype::get_container(const char *name)
  {
    auto it = components.find(TypeDescription::typeDescriptionHash<T>(name));
    return it == components.end() ? dummyContainer : &it->second;
  }

  void Archetype::add_entity(const ComponentInitializerList &list)
  {
    for (auto &component : list.components)
    {
      components[component.first].add_component(component.second.data);
    }
    count++;
  }
  void Archetype::add_entity(const vector<const TemplateInfo*> &list)
  {
    for (const TemplateInfo *component : list)
    {
      components[component->type_name_hash()].add_component(component->data);
    }
    count++;
  }
  void Archetype::destroy_entity(int index, bool without_copy)
  {
    for (auto &container : components)
    {
      container.second.destroy_component(index, without_copy);
    }
    count--;
    if (count < 0)
      debug_error("count < 0 in %s", synonim.c_str());
  }
  
  void Archetype::copy(const Archetype *src)
  {
    for (const auto & component : src->components)
    {
      auto it = components.find(component.first);
      if (it == components.end())
        assert("Tried to copy archetype without need components");

      it->second.copy_components(component.second);
    }
    count = src->count;
  }
}