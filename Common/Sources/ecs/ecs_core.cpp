#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
#include "manager/system_description.h"
#include "common.h"
namespace ecs
{
  Core::Core()
  {
  }
  
  Core::~Core()
  {
    for (Archetype *archetype : archetypes)
    {  
      delete archetype;
    }
  }
  Core &core()
  {
    static Core *singleton = new Core();
    return *singleton;
  }
  void Core::destroy_entities()
  {
    for (auto it = entityContainer.begin(), end = entityContainer.end(); it != end; ++it)
    {
      EntityId eid = it.eid();
      if (eid)
      {
        send_event(eid, OnEntityDestroyed());
        toDestroy.push(eid);
      }
    }
  }



  
  template<typename T>
  T* get_component(const EntityId &entity, const char *name)
  {
    if (entity)
    {
      Archetype& archetype = *core().archetypes[entity.archetype_index()];
      
      ComponentContainer *container = archetype.components[TypeDescription::typeDescription<T>(name).hash()];
      return container ? container->get_component<T>(entity.array_index()) : nullptr;
    }
    return nullptr;
  }


  static void register_archetype_to(QueryDescription *query, Archetype *archetype)
  {
    if (query->args.size() == 0)
      return;
    std::vector<SystemCashedArchetype> &sys_archetypes = query->archetypes;
    std::vector<ComponentContainer*> containers(query->args.size());
    bool breaked = false;
    int i = 0;
    for(auto& arg : query->args)
    {
      ComponentContainer* container = archetype->get_container(arg.descr);
      if (!arg.optional)
      {
        if (container == nullptr || container->typeHash != arg.descr.type_hash())
        {
          breaked = true;
          break;
        }
      }
      containers[i] = container;
      i++;
    }
    if (!breaked)
    {
      sys_archetypes.emplace_back(archetype, std::move(containers));
      //debug_log("processed by %s",query->name.c_str());
    }
  }

  
  void register_archetype(Archetype *archetype)
  {
    for (QueryDescription *query: core().queries)
      register_archetype_to(query, archetype);
    for (SystemDescription *system: core().systems)
      register_archetype_to(system, archetype);
    for (QueryDescription *system: core().event_queries)
      register_archetype_to(system, archetype);
    
  }

  Archetype *add_archetype(const ComponentTypes &types, int capacity)
  {
    Archetype *archetype = new Archetype(types, capacity);
    core().archetypes.push_back(archetype);
    printf("added\n");
    for (const auto &component : archetype->components)
    {
      auto &type = core().types[component.second.typeHash];
      printf("  %s %s\n",type.type.c_str(), type.name.c_str());
    }
    register_archetype(archetype);
    
    return archetype;
  }

  EntityId create_entity(ComponentInitializerList &list)
  {
    list.add<EntityId>("eid") = EntityId();
    Archetype *found_archetype = nullptr;
    int archetype_ind = 0;
    for (Archetype *archetype : core().archetypes)
    {
      if (archetype->in_archetype(list.types))
      {
        found_archetype = archetype;
        break;
      }
      archetype_ind++;
    }
    if (!found_archetype)
    {
      found_archetype = add_archetype(list.types, 1);
    }
    int index = found_archetype->count;
    EntityId eid = core().entityContainer.create_entity(archetype_ind, index);
    list.get<EntityId>("eid") = eid;
    found_archetype->add_entity(list);
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

  bool get_iterator(const QueryDescription &descr, const EntityId &eid, QueryIterator &iterator)
  {
    if (eid)
    {
      int index = eid.array_index();
      int archetypeInd = eid.archetype_index();
      Archetype *archetype = core().archetypes[archetypeInd];

      auto it = std::find_if(descr.archetypes.begin(), descr.archetypes.end(),
        [archetype](const SystemCashedArchetype &cashed_archetype){return cashed_archetype.archetype == archetype;});
      if (it != descr.archetypes.end())
      {
        iterator = QueryIterator(&descr, it - descr.archetypes.begin(), index);
        return true;
      }
    }
    return false;
  }
  bool get_iterator(const SingleQueryDescription &descr, const EntityId &eid, QueryIterator &iterator)
  {
    return get_iterator((const QueryDescription &)descr, eid, iterator);
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
          auto &type = core().types[component.second.typeHash];
          printf("  %s %s\n",type.type.c_str(), type.name.c_str());
        }
      }
      printf("}\n");
    }
  }
}

