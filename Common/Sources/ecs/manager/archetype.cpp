#include "archetype.h"

namespace ecs
{
  static ComponentContainer *dummyContainer = new ComponentContainer();
  
  Archetype::Archetype(const ComponentTypes &types, int capacity):
    components(), count(0), capacity(capacity)
  {
    for(const auto& t : types.componentsTypes)
    {
      auto it = full_description().find(t.type_name_hash());
      assert(it->first && "Don't found full descr for type in Archetype");
      auto typeIt = TypeInfo::types().find(it->second.typeHash);
      assert(typeIt->first && "Don't found this type");
      fullTypeDescriptions.push_back(&it->second);
      components.try_emplace(t.type_name_hash(), typeIt->second.hashId, t.type_name_hash(), capacity, typeIt->second.sizeOf);
    }
  }
  bool Archetype::in_archetype(const ComponentTypes &types)
  {
    if (types.componentsTypes.size() != components.size())
      return false;
    for (const TypeDescription &descr : types.componentsTypes)
    {
      auto it = components.find(descr.type_name_hash());
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
      components[component.first].add_component(component.second);
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
  }
}