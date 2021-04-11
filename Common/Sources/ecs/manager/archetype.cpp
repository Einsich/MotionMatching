#include "archetype.h"

namespace ecs
{
  static ComponentContainer *dummyContainer = new ComponentContainer();
  
  Archetype::Archetype(const ComponentTypes &types, int capacity):
    components(), count(0), capacity(capacity)
  {
    for(const auto& t : types.componentsTypes)
      components.try_emplace(t.hash(), t.typeId, capacity);
  }
  bool Archetype::in_archetype(const ComponentTypes &types)
  {
    if (types.componentsTypes.size() != components.size())
      return false;
    for (const TypeDescription &descr : types.componentsTypes)
    {
      auto it = components.find(descr.hash());
      if (it == components.end())
        return false;
    }
    return true;
  }
  ComponentContainer *Archetype::get_container(const TypeDescription &type)
  {
    auto it = components.find(type.hash());
    return it == components.end() ? dummyContainer : &it->second;
  }
  template<typename T>
  ComponentContainer *Archetype::get_container(const char *name)
  {
    auto it = components.find(TypeDescription::TypeDescription<T>(name).hash());
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
  void Archetype::destroy_entity(int index)
  {
    for (auto &container : components)
    {
      container.second.destroy_component(index);
    }
  }
}