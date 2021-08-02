#include "archetype.h"
#include "../editor/template.h"
#include "Serialization/serialization.h"
#include "entity_pull.h"
#include "../ecs_event.h"
namespace ecs
{
  static ComponentContainer *dummyContainer = new ComponentContainer();

  Archetype::Archetype(int count, const string &synonim, int type_count):
    components(), count(count), capacity(count), fullTypeDescriptions(type_count), synonim(synonim), dontSave(false)
  {}
  
  Archetype::Archetype(const vector<string_hash> &type_hashes, int capacity, const string &synonim):
    components(), count(0), capacity(capacity), fullTypeDescriptions(), synonim(synonim), dontSave(false)
  {
    for (string_hash typeNameHash : type_hashes)
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
    auto it = components.find(type.type_name_hash());
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
  void save(std::ostream& os, const std::vector<Archetype*> &archetypes)
  {
    constexpr string_hash eidTypeHash = HashedString(nameOf<EntityId>::value);
    //constexpr string_hash eidTypeNameHash  = TypeDescription::hash(HashedString("eid"), eidTypeHash);
    size_t size = write(os, archetypes.size());
    vector<int> archetypeRemap(archetypes.size());
    for (uint i = 0, j = 0; i < archetypes.size(); ++i)
    {
      if (archetypes[i] && !archetypes[i]->dontSave)
        archetypeRemap[i] = j++;
      else
        archetypeRemap[i] = -1;
    }
    int archetypeIndex = 0;
    for (const Archetype* archetype : archetypes)
    {
      if (!archetype || archetype->dontSave)
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
            size += typeInfo.serialiser(os, container.get_component<void>(i));
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
  
  template<typename E>
  void send_event(const EntityId &eid, const E &event);
  void register_archetype(Archetype* archetype);

  void load(std::istream& is, std::vector<Archetype*> &archetypes, EntityPull &entityPull)
  {
    entityPull.clear();
    constexpr string_hash eidTypeHash = HashedString(nameOf<EntityId>::value);
    //constexpr string_hash eidTypeNameHash  = TypeDescription::hash(HashedString("eid"), eidTypeHash);
    size_t n = 0;
    size_t size = read(is, n);
    archetypes.resize(n);
    for (size_t k = 0; k < n; ++k)
    {
      int count;
      std::string synonim;
      size_t descrN = 0;
      size += read(is, synonim, count, descrN);
      archetypes[k] = new Archetype(count, synonim, descrN);
      for (size_t i = 0; i < descrN; ++i)
      {
        string typeName, compName;
        size += read(is, typeName, compName);
        uint typeHash = HashedString(typeName);
        uint nameHash = HashedString(compName);
        uint typeNameHash = TypeDescription::hash(nameHash, typeHash);
        auto typeIt = TypeInfo::types().find(typeHash);
        const TypeInfo &typeInfo = typeIt->second;
        auto descrIt = full_description().try_emplace(typeNameHash, compName.c_str(), typeHash, typeNameHash);
        archetypes[k]->fullTypeDescriptions[i] = &descrIt.first->second;
        
        auto p = archetypes[k]->components.try_emplace
          (typeNameHash, typeInfo.hashId, typeNameHash, count, typeInfo.sizeOf);
        assert(p.second && "Failed to emplace type");
        ComponentContainer &container = p.first->second;
        container.count = count;
        if (typeHash != eidTypeHash)
          for (int j = 0; j < count; ++j)
          {
            void *data = container.get_component<void>(j);
            typeInfo.constructor(data);
            size += typeInfo.deserialiser(is, data);
          }
        else
          for (int j = 0; j < count; ++j)
          {
            EntityId *eid = container.get_component<EntityId>(j);
            int archetypeIndex, arrayIndex;
            size += read(is, archetypeIndex, arrayIndex);
            *eid = archetypeIndex > 0 ? entityPull.create_entity(archetypeIndex, arrayIndex) : EntityId();
            if (archetypeIndex == (int)k && arrayIndex == j)
              send_event(*eid, OnEntityCreated());

          }
            
      }
      register_archetype(archetypes[k]);
    }
  }
}