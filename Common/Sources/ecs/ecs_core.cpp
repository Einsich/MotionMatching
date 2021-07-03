#include <algorithm>
#include "ecs_core.h"
#include "ecs_event.h"
#include "manager/system_description.h"
#include "editor/template.h"
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
      
      ComponentContainer *container = archetype.components[TypeDescription::typeDescriptionHash<T>(name)];
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
        if (container == nullptr || container->typeNameHash != arg.descr.type_name_hash())
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
  Archetype *add_archetype(const vector<string_hash> &type_hashes, int capacity, const string &synonim)
  {
    Archetype *archetype = new Archetype(type_hashes, capacity, synonim);
    core().archetypes.push_back(archetype);
    printf("added\n");
    for (const auto &component : archetype->components)
    {
      auto &ecsType = core().types[component.first];
      auto &cppType = TypeInfo::types()[ecsType.typeHash];
      printf("  %s %s\n",cppType.name.c_str(), ecsType.name.c_str());
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
      vector<string_hash> type_hashes(list.types.componentsTypes.size());
      for (uint i = 0; i < list.types.componentsTypes.size(); ++i)
        type_hashes[i] = list.types.componentsTypes[i].type_name_hash();
      found_archetype = add_archetype(type_hashes, 1, "template[" + to_string(core().archetypes.size()) + "]");
    }
    int index = found_archetype->count;
    EntityId eid = core().entityContainer.create_entity(archetype_ind, index);
    list.get<EntityId>("eid") = eid;
    found_archetype->add_entity(list);
    send_event_immediate(eid, OnEntityCreated());

    return eid;
  }
  
  EntityId create_entity(const string &template_name)
  {
    return create_entity(template_name.c_str());
  }
  EntityId create_entity(const char *template_name)
  {
    const Template *t = TemplateManager::get_template_by_name(template_name);
    if (!t)
    {
      debug_error("Doesn't exist template with name %s", template_name);
      return EntityId();
    }
    return create_entity(t);
  }
  EntityId create_entity(const Template *t)
  {
    constexpr string_hash eidHash  = HashedString(nameOf<EntityId>::value);
    static TypeInfo &eidInfo = TypeInfo::types()[eidHash];
    static TemplateInfo eidTemplateInfo(eidHash, eidInfo, "eid");
    vector<const TemplateInfo*> list = linearize_field(t);
    list.push_back(&eidTemplateInfo);

    Archetype *found_archetype = nullptr;
    int archetype_ind = 0;
    for (Archetype *archetype : core().archetypes)
    {
      if (archetype->in_archetype(list))
      {
        found_archetype = archetype;
        break;
      }
      archetype_ind++;
    }
    if (!found_archetype)
    {
      auto &fullDecr = full_description();
      for (const TemplateInfo *t: list)
        fullDecr.try_emplace(t->type_name_hash(), t->get_name(), t->type_hash(), t->type_name_hash());
        
      vector<string_hash> type_hashes(list.size());
      for (uint i = 0; i < list.size(); ++i)
        type_hashes[i] = list[i]->type_name_hash();
      found_archetype = add_archetype(type_hashes, 1, t->name);
    }
    int index = found_archetype->count;
    EntityId eid = core().entityContainer.create_entity(archetype_ind, index);
    eidInfo.copy_constructor(&eid, eidTemplateInfo.data); 
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
          auto &ecsType = core().types[component.first];
          auto &cppType = TypeInfo::types()[ecsType.typeHash];
          printf("  %s %s\n",cppType.name.c_str(), ecsType.name.c_str());
        }
      }
      printf("}\n");
    }
  }
}

