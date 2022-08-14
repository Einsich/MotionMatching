#include "archetype.h"
#include "entity_pull.h"
#include "entity_id.h"
#include "string_hash.h"
#include "common.h"
namespace ecs
{
  Archetype::Archetype(int index, const ecs::vector<ComponentInstance> &type_hashes, int capacity, const ecs::string &synonim):
    index(index), components(), count(0), capacity(capacity), typeDescriptions(), synonim(synonim)
  {
    for (const ComponentInstance &instance : type_hashes)
    {
      #if ECS_USE_EASTL
      auto result = components.insert(instance.nameHash);
      if (result.second)
      {
        result.first->second = ComponentContainer(*instance.typeInfo, capacity);
      #else
      if (components.try_emplace(instance.nameHash, *instance.typeInfo, capacity).second)
      {
      #endif
        typeDescriptions.emplace_back(TypeDescription{instance.name, instance.typeInfo, nullptr, instance.nameHash});
      }
    }

    for (auto &descr : typeDescriptions)
      descr.components = &components[descr.nameHash];

    std::sort(typeDescriptions.begin(), typeDescriptions.end(),
      [](const auto &a, const auto &b) {
        return a.name < b.name;
      });
  }

  bool Archetype::in_archetype(const ecs::vector<ComponentInstance> &instances) const
  {
    if (instances.size() != components.size())
      return false;
    for (const ComponentInstance &descr : instances)
    {
      auto it = components.find(descr.nameHash);
      if (it == components.end())
        return false;
    }
    return true;
  }

  ComponentContainer *Archetype::get_container(uint name_hash)
  {
    auto it = components.find(name_hash);
    return it == components.end() ? nullptr : &it->second;
  }

  void Archetype::destroy_entity(int index)
  {
    if (count > 0)
    for (auto &container : components)
    {
      container.second.destroy_component(index);
    }
    count--;
    if (index != count)
    {
      ComponentContainer &eidContainer = components[HashedString("eid")];
      assert(eidContainer.typeHash == ecs::type_hash<EntityId>());
      EntityId &eid = *eidContainer.get_component<EntityId>(index);
      eid.migrate(eid.archetype_index(), index);
    }
    if (count < 0)
      ECS_LOG("count < 0 in %s", synonim.c_str());
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
  void Archetype::clear()
  {
    for (auto & component : components)
    {
      component.second.clear();
    }
    count = 0;
  }
}