#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
#include "manager/system_description.h"
#include "common.h"
#include "manager/entity_manager.h"
#include "template.h"
#include "ecs_tag.h"
#include "ecs_event_impl.h"
#define ECS_DEBUG_INFO 0
namespace ecs
{

  EntityManager *entityManager = nullptr;

  void clear_system_archetypes_cache();

  void set_entity_manager(EntityManager &manager)
  {
    if (entityManager)
    {
      invalidate_cached_archetype();
      clear_system_archetypes_cache();

    }
    entityManager = &manager;
    manager.resolve_system_order_and_subscribes();

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
      if (arg.typeHash != 0)//singleton case
      {
        ComponentContainer* container = archetype->get_container(arg.nameHash);
        if (!arg.optional && (container == nullptr || (arg.typeHash != -1u && container->typeHash != arg.typeHash)))
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
      if (archetype->get_container(arg.nameHash) != nullptr)
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
    Archetype *archetype = new Archetype(entityManager->archetypes.size(), tmpl.components, 1, tmpl.name);
    entityManager->archetypes.emplace_back(archetype);

    register_archetype(archetype);

    return archetype;
  }



  EntityId EntityManager::create_entity(const char *template_name, ComponentInitializerList &&list)
  {
    return create_entity(ecs::get_template(template_name), std::move(list));
  }

  EntityId EntityManager::create_entity(const Template *tmpl, ComponentInitializerList &&list)
  {
    if (!tmpl)
      return EntityId();
    if (!tmpl->archetype)
    {
      extern void update_template_cache(Template &tmpl);
      update_template_cache((Template &)(*tmpl));
    }
    Archetype *found_archetype = tmpl->archetype;
    int index = found_archetype->count;
    int archetype_ind = found_archetype->index;
    EntityId eid = entityManager->entityPull.create_entity(archetype_ind, index);


    list.emplace_back("eid", EntityId(eid));
    {
      const vector<ComponentInstance> &template_list = tmpl->components;
      for (size_t i = 0, n = template_list.size(); i < n; ++i)
      {
        ComponentInstance &instance = (ComponentInstance &)template_list[i];
        ComponentContainer *container = tmpl->containers[i];
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

  void EntityManager::destroy_entity(const EntityId &eid)
  {
    if (eid)
    {
      entityManager->toDestroy.push(eid);
    }
  }
  EntityId EntityManager::find_entity(uint archetype, uint index)
  {
    return entityManager->entityPull.find_entity(archetype, index);
  }

  EntityId create_entity(const Template *temp, ComponentInitializerList &&list)
  {
    return entityManager->create_entity(temp, std::move(list));
  }
  EntityId create_entity(const char *template_name, ComponentInitializerList &&list)
  {
    return entityManager->create_entity(ecs::get_template(template_name), std::move(list));
  }
  EntityId find_entity(uint archetype, uint index)
  {
    return entityManager->find_entity(archetype, index);
  }

  void destroy_entity(const EntityId &eid)
  {
    return entityManager->destroy_entity(eid);
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
    for (const auto &archetype : entityManager->archetypes)
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

}

