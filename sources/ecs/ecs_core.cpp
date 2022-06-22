#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
#include "manager/system_description.h"
#include "common.h"
#include "manager/entity_container.h"
#include "template.h"
#include "ecs_tag.h"
#include "ecs_event_impl.h"
#define ECS_DEBUG_INFO 0
namespace ecs
{
  Core::Core()
  {
    entityContainer = new EntityContainer();
    sceneToLoad = "";
    applicationTags.clear();
#ifndef RELEASE
    applicationTags.push_back("debug");
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
        ECS_LOG("destroy %d %d entity", eid.archetype_index(), eid.array_index());
        core().entityContainer->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), with_swap_last_element);
        core().entityContainer->entityPull.destroy_entity(eid);
      }
      toDestroy.pop();
    }
  }
  
  void system_sort(const ecs::string &currentSceneTags, const ecs::vector<ecs::string> &applicationTags);
  void Core::resolve_system_order_and_subscribes()
  {
    system_sort(currentSceneTags, applicationTags);
  }


  void register_archetype(Archetype *archetype);
  void Core::update_systems_subscribes()
  {
    core().events = {};
    
    for (auto &archetype : entityContainer->archetypes)
      register_archetype(archetype.get());
  }



  static void register_archetype_to(CallableDescription &query, Archetype *archetype)
  {
    if (query.notSingletonArgsCount == 0)
      return;
    ecs::vector<SystemCashedArchetype> &sys_archetypes = query.archetypes;
    ecs::vector<ComponentContainer*> containers(query.requireArgs.size(), nullptr);
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
    for (const auto& arg : query.requireNotArgs)
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
      ECS_LOG("processed by %s",query->name.c_str());
#endif
    }
  }

  
  void register_archetype(Archetype *archetype)
  {
    
#if ECS_DEBUG_INFO
    ECS_LOG("register archetype");
    for (const auto &[name, typeInfo, components, hash] : archetype->typeDescriptions)
    {
      printf("  %s %s;", typeInfo->name.c_str(), name.c_str());
    }
#endif
    for (QueryDescription *query: ecs::all_queries())
      register_archetype_to(*query, archetype);
    for (SystemDescription *system: ecs::get_all_systems())
      register_archetype_to(*system, archetype);

    for (auto &[srcHandlers, filteredHandlers] : get_all_event_handlers())
    {
      for (EventDescription *handler : filteredHandlers)
        register_archetype_to(*handler, archetype);
    }
    
  }
  
  Archetype *add_archetype(const Template &tmpl)
  {
    Archetype *archetype = new Archetype(core().entityContainer->archetypes.size(), tmpl.components, 1, tmpl.name);
    core().entityContainer->archetypes.emplace_back(archetype);

    register_archetype(archetype);
    
    return archetype;
  }

  

  EntityId create_entity(const char *template_name, ComponentInitializerList &&list)
  {
    return create_entity(ecs::get_template(template_name), std::move(list));
  }

  EntityId create_entity(const Template *blkTemplate, ComponentInitializerList &&list)
  {
    if (!blkTemplate)
      return EntityId();
    if (!blkTemplate->archetype)
    {
      extern void update_template_cache(Template &tmpl);
      update_template_cache((Template &)(*blkTemplate));
    }
    Archetype *found_archetype = blkTemplate->archetype;
    int index = found_archetype->count;
    int archetype_ind = found_archetype->index;
    EntityId eid = core().entityContainer->entityPull.create_entity(archetype_ind, index);


    list.emplace_back("eid", EntityId(eid));
    {
      const vector<ComponentInstance> &template_list = blkTemplate->components;
      for (size_t i = 0, n = template_list.size(); i < n; ++i)
      {
        ComponentInstance &instance = (ComponentInstance &)template_list[i];
        ComponentContainer *container = blkTemplate->containers[i];
        size_t j = 0, m = list.size();
        for (; j < m; ++j)
          if (instance.typeNameHash == list[j].typeNameHash)
            break;

        void *rawMem = container->add_component();
        if (j == m)
        {
          if (instance.initManager)
          {
            instance.init_from_func();
          }
          container->rtti.copy_constructor(instance.get_data(), rawMem);
        }
        else
        {
          if (list[j].initManager)
          {
            list[j].initManager(rawMem);
          }
          else
          {
            container->rtti.move_constructor(list[j].get_data(), rawMem);
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
    ECS_LOG("\nSystems statistics");
    for (const SystemDescription *descr : ecs::get_all_systems())
    {
      int archetypesCount = descr->archetypes.size();
      ECS_LOG("%s has %d archetypes", descr->name.c_str(), archetypesCount);
      printf("{\n");
      for (const SystemCashedArchetype &archetype : descr->archetypes)
      {
        printf("---\n");
        for (const auto &[name, typeInfo, components, hash] : archetype.archetype->typeDescriptions)
        {
          printf("  %s %s\n", typeInfo->name.c_str(), name.c_str());
        }
      }
      printf("}\n");
    }
  }
  
  void print_archetypes()
  {   
    printf("\n- - - - - \n");
    for (const auto &archetype : core().entityContainer->archetypes)
    {
      printf("{\n");
      for (const auto &[name, typeInfo, components, hash] : archetype->typeDescriptions)
      {
        printf("  %s %s\n", typeInfo->name.c_str(), name.c_str());
      }
      printf("},\n");
    }
    std::fflush(stdout);
  }

  void destroy_scene()
  {    
    core().destroy_all_entities();
    core().events = {};
  }
  void create_scene(const char *path, bool reload)
  {
    core().sceneToLoad = path;
    core().reloadScene = reload;
  }
}

