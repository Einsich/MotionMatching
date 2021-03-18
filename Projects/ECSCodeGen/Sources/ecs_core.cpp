#include "ecs_core.h"
#include "ecs_types.h"
#include "system_description.h"
namespace ecs
{
  static int globalTypeIndex = 0;
  vector<TypeDescription> types;
  vector<Archetype> archetypes;

  unsigned int Entity::archetype_index() const
  {
    return index >> 16;
  }
  unsigned int Entity::array_index() const
  {
    return index & index_mask;
  }

  Archetype::Archetype(vector<pair<string, pair<TypeDescription, int>>> types)
  {
    for(auto& t : types)
      components.emplace(t.first, ComponentContainer(t.second.first, t.second.second));
  }
  ComponentContainer* Archetype::get_container(const string& name)
  {
    auto it = components.find(name);
    return it == components.end() ? nullptr : &it->second;
  }
  ComponentContainer::ComponentContainer(const TypeDescription& type, int count):
  data(malloc(count * type.size)), type(type), componentCount(count)
  {  }
  void* ComponentContainer::get_component(int i)
  {
    if (0 <= i && i < componentCount)
      return ((char*)data + i * type.size);
    return nullptr;
  }
  const TypeDescription& ComponentContainer::get_description() const
  {
    return type;
  }
  template<typename T>
  T* get_component(Entity entity, const string& name)
  {
    Archetype& archetype = archetypes[entity.archetype_index()];
    return archetype.components[name].get_component(entity.array_index());
  }

  template<typename T>
  int type_index()
  {
    static int index = globalTypeIndex++;
    return index;
  };


  void initialize_ecs()
  {
    #define TYPE_HANDLER(T) \
    types.push_back(TypeDescription(#T, sizeof(T), type_index<T>()));

    ECS_TYPES 
    
    //init archetypes??

  }
  SystemArchetypes register_system(const SystemDesription& descr)
  {
    SystemArchetypes sys_archetypes;
    for (Archetype& archetype: archetypes)
    {
      vector<ComponentContainer*> containers(descr.args.size());
      bool breaked = false;
      int i = 0;
      for(auto& arg : descr.args)
      {
        ComponentContainer* container = archetype.get_container(arg.name);
        if (!arg.optional)
        {
          if (container == nullptr || container->get_description().name != arg.name)
          {
            breaked = true;
            break;
          }
        }
        containers[i] = container;
        i++;
      }
      if (!breaked)
        sys_archetypes.archetypes.emplace_back(containers);
    }
    return sys_archetypes;
  }


}