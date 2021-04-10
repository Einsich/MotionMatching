#include "ecs_core.h"
#include "ecs_types.h"
#include "system_description.h"
#include "ecs/test_header.h"
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
  void initialize_ecs()
  {
    
    ComponentTypes c1 = {{get_type_description<int>("a"), get_type_description<float>("b")}};
    ComponentTypes c2 = {{get_type_description<std::string>("s"), get_type_description<float>("f")}};
    ComponentTypes c3 = {{get_type_description<A>("v"), get_type_description<B>("w")}};
    ComponentTypes c4 = {{get_type_description<A>("v")}};
    core().archetypes.push_back(new Archetype(c1, 1));
    core().archetypes.push_back(new Archetype(c2, 2));
    core().archetypes.push_back(new Archetype(c3, 1));
    core().archetypes.push_back(new Archetype(c4, 1));

    for (Archetype * archetype : core().archetypes)
      register_archetype(archetype);

  }

  void update_systems()
  {
    for (SystemDescription *system : core().systems)
    {
      system->execute();
    }
  }

  void free_ecs()
  {
  }
}

