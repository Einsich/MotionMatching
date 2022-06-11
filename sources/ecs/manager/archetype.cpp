#include "archetype.h"
#include "serialization/serialization.h"
#include "entity_pull.h"
#include "core_interface.h"
#include "entity_id.h"
#include "string_hash.h"
#include "../ecs_event.h"
namespace ecs
{

  Archetype::Archetype(int index, int count, const string &synonim, int type_count):
    index(index), components(), count(count), capacity(count), fullTypeDescriptions(type_count), synonim(synonim), dontSave(false)
  {}
  
  Archetype::Archetype(int index, const vector<uint> &type_hashes, int capacity, const string &synonim):
    index(index), components(), count(0), capacity(capacity), fullTypeDescriptions(), synonim(synonim), dontSave(false)
  {
    for (uint typeNameHash : type_hashes)
    {
      auto it = full_description().find(typeNameHash);
      assert(it->first && "Don't found full descr for type in Archetype");
      auto typeIt = TypeInfo::types().find(it->second.typeHash);
      assert(typeIt->first && "Don't found this type");
      fullTypeDescriptions.push_back(&it->second);
      auto result = components.insert(typeNameHash);
      if (result.second)
      {
        result.first->second = ComponentContainer(*typeIt->second, typeNameHash, capacity);
      }
    }    
  }

  bool Archetype::in_archetype(const vector<uint> &type_hashes) const
  {
    if (type_hashes.size() != components.size())
      return false;
    for (uint descr : type_hashes)
    {
      auto it = components.find(descr);
      if (it == components.end())
        return false;
    }
    return true;
  }
  ComponentContainer *Archetype::get_container(const TypeDescription &type)
  {
    auto it = components.find(type.type_name_hash());
    return it == components.end() ? nullptr : &it->second;
  }
  template<typename T>
  ComponentContainer *Archetype::get_container(const char *name)
  {
    auto it = components.find(TypeDescription::typeDescriptionHash<T>(name));
    return it == components.end() ? nullptr : &it->second;
  }

  vector<void*> Archetype::get_entity_data(const vector<uint> &type_hashes)
  {
    vector<void*> data(type_hashes.size());
    for (uint i = 0; i < type_hashes.size(); i++)
      data[i] = components[type_hashes[i]].add_component();
    return data;
  }
  void Archetype::destroy_entity(int index, bool with_swap)
  {
    if (count > 0)
    for (auto &container : components)
    {
      container.second.destroy_component(index, with_swap);
    }
    count--;
    if (with_swap && index != count)
    {
      EntityId &eid = *components[get_type_description<EntityId>("eid")]
      .get_component<EntityId>(index);
      eid.migrate(eid.archetype_index(), index);
    }
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
  void save(std::ostream& os, const std::vector<Archetype*> &archetypes)
  {
    constexpr uint eidTypeHash = HashedString("EntityId");
    //constexpr uint eidTypeNameHash  = TypeDescription::hash(HashedString("eid"), eidTypeHash);
    size_t archetypeToSave = 0;
    vector<int> archetypeRemap(archetypes.size());
    for (uint i = 0; i < archetypes.size(); ++i)
    {
      if (archetypes[i] && !archetypes[i]->dontSave && archetypes[i]->count > 0)
        archetypeRemap[i] = archetypeToSave++;
      else
        archetypeRemap[i] = -1;
    }    
    size_t size = write(os, archetypeToSave);
    int archetypeIndex = 0;
    for (const Archetype* archetype : archetypes)
    {
      if (!archetype || archetype->dontSave || archetype->count <= 0)
        continue;
      assert(archetype->fullTypeDescriptions.size() == archetype->components.size());

      size += write(os, archetype->synonim, archetype->count, archetype->fullTypeDescriptions.size());
      for (const FullTypeDescription *fullDesc : archetype->fullTypeDescriptions)
      {
        auto it = archetype->components.find(fullDesc->hash);
        const ComponentContainer &container = it->second;
        const TypeInfo &typeInfo = fullDesc->get_type_info();
        size += write(os, typeInfo.name, fullDesc->name);
        if (fullDesc->typeHash != eidTypeHash)
          for (int i = 0; i < archetype->count; ++i)
            size += typeInfo.userInfo.serialiser(os, container.get_component<void>(i));
        else
          for (int i = 0; i < archetype->count; ++i)
          {
            const EntityId *eid = container.get_component<EntityId>(i);
            int archetypeIndex = eid->valid() ? archetypeRemap[eid->archetype_index()] : -1;
            int arrayIndex = eid->array_index();
            size += write(os, archetypeIndex, arrayIndex);
          }

      }
      archetypeIndex++;
    }
  }
}