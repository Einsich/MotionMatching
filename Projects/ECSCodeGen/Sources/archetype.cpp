#include "archetype.h"

namespace ecs
{
  static ComponentContainer *dummyContainer = new ComponentContainer();
  
  Archetype::Archetype(const ComponentTypes &types, int count):
    components(), componentCount(count)
  {
    for(const auto& t : types.componentsTypes)
    components.try_emplace(t.hash(), t.typeId, count);
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
}