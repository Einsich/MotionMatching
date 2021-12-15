#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
#include "system_tag.h"
#include "manager/system_description.h"
#include "common.h"
#include "manager/entity_container.h"
#include "template/blk_template.h"

#define ECS_DEBUG_INFO 0
namespace ecs
{
  Core::Core()
  {
    entityContainer = new EntityContainer();
    sceneToLoad = "";
  }
  
  Core::~Core()
  {
  }
  Core &core()
  {
    static Core *singleton = new Core();
    return *singleton;
  }
  void Core::destroy_all_entities()
  {
    destroy_entities_from_destroy_queue(false);
    for (auto it = entityContainer->entityPull.begin(), end = entityContainer->entityPull.end(); it != end; ++it)
    {
      EntityId eid = it.eid();
      if (eid)
      {
        send_event_immediate(eid, OnEntityDestroyed());
        entityContainer->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), false);
      }
    }
    entityContainer->entityPull.clear();
  }
  
  void Core::destroy_entities_from_destroy_queue(bool with_swap_last_element)
  {
    for (int i = 0, n = toDestroy.size(); i < n; i++)
    {
      EntityId &eid = toDestroy.front();
      if (eid)
      {
        debug_log("destroy %d %d entity", eid.archetype_index(), eid.array_index());
        core().entityContainer->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), with_swap_last_element);
        core().entityContainer->entityPull.destroy_entity(eid);
      }
      toDestroy.pop();
    }
  }
  
  bool Core::allow_system_execute(uint tags, uint require_tags)
  {
#ifdef RELEASE
    if (tags & SystemTag::Debug)
      return false;
#endif
    return (tags & require_tags) == require_tags;
  }


  void register_archetype(Archetype *archetype);
  void Core::replace_entity_container(EntityContainer *container)
  {
    core().events = {};
    for (QueryDescription *query: core().queries)
      query->archetypes.clear();
    for (SystemDescription *system: core().systems)
      system->archetypes.clear();
    for (QueryDescription *system: core().event_queries)
      system->archetypes.clear();
    entityContainer = container;
    for (Archetype *archetype : container->archetypes)
      register_archetype(archetype);
  }


  
 /*  template<typename T>
  T* get_component(const EntityId &entity, const char *name)
  {
    if (entity)
    {
      Archetype& archetype = *core().archetypes[entity.archetype_index()];
      
      ComponentContainer *container = archetype.components[TypeDescription::typeDescriptionHash<T>(name)];
      return container ? container->get_component<T>(entity.array_index()) : nullptr;
    }
    return nullptr;
  } */


  static void register_archetype_to(QueryDescription *query, Archetype *archetype)
  {
    if (!query->withArgs)
      return;
    std::vector<SystemCashedArchetype> &sys_archetypes = query->archetypes;
    std::vector<ComponentContainer*> containers(query->args.size(), nullptr);
    bool breaked = false;
    int i = 0;
    for(auto& arg : query->args)
    {
      if (arg.descr != 0)//singleton case
      {
        ComponentContainer* container = archetype->get_container(arg.descr);
        if (!arg.optional)
        {
          if (container == nullptr || container->typeNameHash != arg.descr)
          {
            breaked = true;
            break;
          }
        }
        containers[i] = container;
      }
      i++;
    }
    if (!breaked)
    {
      sys_archetypes.emplace_back(archetype, std::move(containers));
#if ECS_DEBUG_INFO
      debug_log("processed by %s",query->name.c_str());
#endif
    }
  }

  
  void register_archetype(Archetype *archetype)
  {
    
#if ECS_DEBUG_INFO
    debug_log("register archetype");
    for (const auto &component : archetype->components)
    {
      auto &ecsType = core().types[component.first];
      auto &cppType = TypeInfo::types()[ecsType.typeHash];
      debug_log("  %s %s",cppType.name.c_str(), ecsType.name.c_str());
    }
#endif
    for (QueryDescription *query: core().queries)
      register_archetype_to(query, archetype);
    for (SystemDescription *system: core().systems)
      register_archetype_to(system, archetype);
    for (QueryDescription *system: core().event_queries)
      register_archetype_to(system, archetype);
    
  }
  Archetype *add_archetype(const vector<string_hash> &type_hashes, int capacity, const string &synonim)
  {
    Archetype *archetype = new Archetype(core().entityContainer->archetypes.size(), type_hashes, capacity, synonim);
    core().entityContainer->archetypes.push_back(archetype);

    register_archetype(archetype);
    
    return archetype;
  }

  

  pair<EntityId, Archetype&> add_entity(const vector<string_hash> & type_hashes)
  {
    Archetype *found_archetype = nullptr;
    int archetype_ind = 0;
    for (Archetype *archetype : core().entityContainer->archetypes)
    {
      if (archetype->in_archetype(type_hashes))
      {
        found_archetype = archetype;
        break;
      }
      archetype_ind++;
    }
    if (!found_archetype)
    {
      found_archetype = add_archetype(type_hashes, 1, "template[" + to_string(core().entityContainer->archetypes.size()) + "]");
    }
    int index = found_archetype->count++;
    return {core().entityContainer->entityPull.create_entity(archetype_ind, index), *found_archetype};
  }

  EntityId create_entity(const Template *blkTemplate, ComponentInitializerList &&list)
  {
    if (!blkTemplate)
      return EntityId();
    Archetype *found_archetype = blkTemplate->archetype;
    if (!blkTemplate->archetype)
    {
      size_t templateComponentsCount = blkTemplate->components.size();
    
      vector<string_hash> typeHashes(templateComponentsCount);
      
      for (uint i = 0; i < templateComponentsCount; ++i)
        typeHashes[i] = blkTemplate->components[i].typeNameHash;

      for (Archetype *archetype : core().entityContainer->archetypes)
      {
        if (archetype->in_archetype(typeHashes))
        {
          found_archetype = archetype;
          break;
        }
      }
      if (!found_archetype)
      {
        auto &fullDecr = full_description();
        for (const ComponentInstance &t: blkTemplate->components)
          fullDecr.try_emplace(t.typeNameHash, t.name.c_str(), t.typeInfo->hashId, t.typeNameHash);
        blkTemplate->archetype  = found_archetype = add_archetype(typeHashes, 1, blkTemplate->name);
      }
      size_t count = blkTemplate->components.size();
      blkTemplate->containers.reserve(count);
      for (const ComponentInstance &instance : blkTemplate->components)
      {
        blkTemplate->containers.emplace_back(&found_archetype->components[instance.typeNameHash]);
      }
    }
    int index = found_archetype->count;
    int archetype_ind = found_archetype->index;
    EntityId eid = core().entityContainer->entityPull.create_entity(archetype_ind, index);
    constexpr string_hash eidHash = HashedString(nameOf<EntityId>::value);
    static TypeInfo &eidInfo = TypeInfo::types()[eidHash];
    list.components.emplace_back(eidInfo, "eid", eid);
    {
      const vector<ComponentInstance> &template_list = blkTemplate->components;
      vector<ComponentInstance> &&init_list = std::move(list.components);
      for (size_t i = 0, n = template_list.size(); i < n; ++i)
      {
        const ComponentInstance &instance = template_list[i];
        ComponentContainer *container = blkTemplate->containers[i];
        size_t j = 0, m = init_list.size();
        for (; j < m; ++j)
        {
          if (instance.typeNameHash == init_list[j].typeNameHash)
          {
            break;
          }
        }
        if (j == m)
          container->add_component(instance.get_data());
        else
          container->add_component(init_list[j].get_data());
      }
      found_archetype->count++;
    }
    send_event_immediate(eid, OnEntityCreated());
    return eid;
  }

  void create_entities_from_archetypes(const vector<Archetype *> &src)
  {
    constexpr string_hash eidTypeHash = HashedString(nameOf<EntityId>::value);
    constexpr string_hash eidTypeNameHash  = TypeDescription::hash(HashedString("eid"), eidTypeHash);
    vector<int> remapArchetypes(src.size(), -1);//src index to real index

      core().entityContainer->entityPull.clear();
    int j = 0;
    for (const Archetype *archetype : src)
    {
      vector<string_hash> typeHashes(archetype->components.size());
      int i = 0;
      for (const auto &component : archetype->components)
        typeHashes[i++] = component.second.typeNameHash;

      Archetype *found_archetype = nullptr;
      int archetype_ind = 0;
      for (Archetype *archetype : core().entityContainer->archetypes)
      {
        if (archetype->in_archetype(typeHashes))
        {
          found_archetype = archetype;
          break;
        }
        archetype_ind++;
      }
      if (!found_archetype)
      {
        found_archetype = add_archetype(typeHashes, archetype->count, archetype->synonim);
      }
      assert(found_archetype->count == 0 && "need empty archetype");
      remapArchetypes[j++] = archetype_ind;
    }
    
    for (uint i = 0; i < src.size(); ++i)
    {
      Archetype *archetype = src[i];
      if (remapArchetypes[i] < 0)
        assert("Can't remap this archetype ");
      Archetype *found_archetype = core().entityContainer->archetypes[remapArchetypes[i]];
      
      found_archetype->copy(archetype);
      auto eidIt = found_archetype->components.find(eidTypeNameHash);
      if (eidIt == found_archetype->components.end())
        assert("need ecs::EntityId eid containner in existing archetype");
      ComponentContainer &eidContainer = eidIt->second;
      for (int j = 0; j < archetype->count; ++j)
      {
        EntityId *eid = eidContainer.get_component<EntityId>(j);
        *eid = core().entityContainer->entityPull.create_entity(remapArchetypes[i], j);
        debug_log("cr entity %d %d", eid->archetype_index(), eid->array_index());
        send_event(*eid, OnEntityCreated());
      }
    }
    for (Archetype *archetype :core().entityContainer->archetypes)
    {
      for (auto &p : archetype->components)
        if (p.second.typeHash == eidTypeHash && p.second.typeNameHash != eidTypeNameHash)
        {
          for (int i = 0; i < archetype->count; ++i)
          {
            EntityId *eid = p.second.get_component<EntityId>(i);
            debug_log("entity %d %d", eid->archetype_index(), eid->array_index());
            fflush(stdout);
            if (eid->valid())
            {
              int arch = remapArchetypes[eid->archetype_index()];
              assert(arch >= 0);
              int ind = eid->array_index();
              *eid = *core().entityContainer->archetypes[arch]->components[eidTypeNameHash].get_component<EntityId>(ind);
            }
          }
        }
    }
  }
  void destroy_entity(const EntityId &eid)
  {
    if (eid)
    {
      send_event_immediate(eid, OnEntityDestroyed());
      core().toDestroy.push(eid);
    }
  }
  EntityId find_entity(uint archetype, uint index)
  {
    return core().entityContainer->entityPull.find_entity(archetype, index);
  }

  void system_statistic()
  {
    debug_log("\nSystems statistics");
    for (const SystemDescription *descr : core().systems)
    {
      int archetypesCount = descr->archetypes.size();
      debug_log("%s has %d archetypes", descr->name.c_str(), archetypesCount);
      printf("{\n");
      for (const SystemCashedArchetype &archetype : descr->archetypes)
      {
        printf("---\n");
        for (const auto &component : archetype.archetype->components)
        {
          auto &ecsType = core().types[component.first];
          auto &cppType = TypeInfo::types()[ecsType.typeHash];
          printf("  %s %s\n",cppType.name.c_str(), ecsType.name.c_str());
        }
      }
      printf("}\n");
    }
  }
  void destroy_scene()
  {    
    core().destroy_all_entities();
    core().events = {};
  }
  void create_scene(const string &path, bool reload)
  {
    core().sceneToLoad = path;
    core().reloadScene = reload;
  }
}

