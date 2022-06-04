#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
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
    applicationTags = 0;
#ifndef RELEASE
    applicationTags |= tags::debug;
#endif
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
  
  void Core::register_allowed_callable()
  {
    systems.clear();
    queries.clear();
    event_queries.clear();
    for (const auto &cleaner : events_cleaners)
      cleaner();
    for (CallableDescription *callable : all_callable)
    {
      if ((callable->tags & applicationTags) == callable->tags &&
        (callable->isQuery ||
          (callable->scenes.empty() && currentSceneTags != "editor") || 
        std::find(callable->scenes.begin(), callable->scenes.end(), currentSceneTags)
          != callable->scenes.end()))
          callable->registration();
    }
      
  }


  void register_archetype(Archetype *archetype);
  void Core::update_systems_subscribes()
  {
    core().events = {};
    for (QueryDescription *query: core().queries)
      query->archetypes.clear();
    for (SystemDescription *system: core().systems)
      system->archetypes.clear();
    for (CallableDescription *system: core().event_queries)
      system->archetypes.clear();

    for (Archetype *archetype : entityContainer->archetypes)
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


  static void register_archetype_to(CallableDescription &query, Archetype *archetype)
  {
    if (query.notSingletonArgsCount == 0)
      return;
    std::vector<SystemCashedArchetype> &sys_archetypes = query.archetypes;
    std::vector<ComponentContainer*> containers(query.requireArgs.size(), nullptr);
    bool breaked = false;
    int i = 0;
    for(const auto& arg : query.requireArgs)
    {
      if (arg.descr != 0)//singleton case
      {
        ComponentContainer* container = archetype->get_container(arg.descr);
        if (!arg.optional && (container == nullptr || container->typeNameHash != arg.descr))
        {
          breaked = true;
          break;
        }
        containers[i] = container;
      }
      i++;
    }
    for(const auto& arg : query.requireNotArgs)
    {
      if (arg.descr != 0 && archetype->get_container(arg.descr) != nullptr)//singleton case
      {
        breaked = true;
        break;
      }
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
      register_archetype_to(*query, archetype);
    for (SystemDescription *system: core().systems)
      register_archetype_to(*system, archetype);
    for (CallableDescription *system: core().event_queries)
      register_archetype_to(*system, archetype);
    
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
  EntityId create_entity(const Template *blkTemplate)
  {
    return create_entity(blkTemplate, {});
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
        void *rawMem = container->add_component();
        if (j == m)
        {
          if (instance.initManager)
          {
            instance.initManager(rawMem);
          }
          else
          {
            container->copy_constructor(instance.get_data(), rawMem);
          }
        }
        else
        {
          if (init_list[j].initManager)
          {
            init_list[j].initManager(rawMem);
          }
          else
          {
            container->move_constructor(init_list[j].get_data(), rawMem);
          }
        }
      }
      found_archetype->count++;
    }
    send_event_immediate(eid, OnEntityCreated());
    return eid;
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

