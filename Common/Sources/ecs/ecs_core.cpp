#include "ecs_core.h"
#include "manager/system_description.h"
namespace ecs
{
  
  Core::~Core()
  {
    for (Archetype *archetype : archetypes)
      archetype->~Archetype();
  }
  Core &core()
  {
    static Core *singleton = new Core();
    return *singleton;
  }

  
  
  


  unsigned int Entity::archetype_index() const
  {
    return index >> 16;
  }
  unsigned int Entity::array_index() const
  {
    return index & index_mask;
  }

  
  
  template<typename T>
  T* get_component(Entity entity, const char *name)
  {
    Archetype& archetype = core().archetypes[entity.archetype_index()];
    
    ComponentContainer *container = archetype.components[TypeDescription::TypeDescription<T>(name).hash()];
    return container ? container->get_component<T>(entity.array_index()) : nullptr;
  }




  
  void register_archetype(Archetype *archetype)
  {
    for (SystemDescription *system: core().systems)
    {
      std::vector<SystemCashedArchetype> &sys_archetypes = system->archetypes;
      std::vector<ComponentContainer*> containers(system->args.size());
      bool breaked = false;
      int i = 0;
      for(auto& arg : system->args)
      {
        ComponentContainer* container = archetype->get_container(arg.descr);
        if (!arg.optional)
        {
          if (container == nullptr || container->typeID != arg.descr.typeId)
          {
            breaked = true;
            break;
          }
        }
        containers[i] = container;
        i++;
      }
      if (!breaked)
        sys_archetypes.emplace_back(archetype, std::move(containers));
    }
    
  }

  Archetype *add_archetype(const ComponentTypes &types, int capacity)
  {
    Archetype *archetype = new Archetype(types, capacity);
    core().archetypes.push_back(archetype);
    register_archetype(archetype);
    return archetype;
  }
  void initialize_ecs()
  {
    
    

  }

  void update_systems()
  {
    for (SystemDescription *system : core().systems)
    {
      system->execute();
    }
  }
  void create_entity(const ComponentInitializerList &list)
  {
    Archetype *found_archetype = nullptr;
    for (Archetype *archetype : core().archetypes)
    {
      if (archetype->in_archetype(list.types))
      {
        found_archetype = archetype;
        break;
      }
    }
    if (!found_archetype)
    {
      found_archetype = add_archetype(list.types, 1);
    }
    found_archetype->add_entity(list);
  }

  void free_ecs()
  {
  }
}

