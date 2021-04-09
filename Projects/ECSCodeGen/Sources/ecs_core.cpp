#include "ecs_core.h"
#include "ecs_types.h"
#include "system_description.h"
namespace ecs
{
  static uint globalTypeIndex = 0;
  vector<FullTypeDescription> types;
  vector<Archetype*> archetypes;

  std::unordered_map<string_hash, int> l;

  unsigned int Entity::archetype_index() const
  {
    return index >> 16;
  }
  unsigned int Entity::array_index() const
  {
    return index & index_mask;
  }

  Archetype::Archetype(const ComponentTypes &types, int count)
  {
    for(const auto& t : types.componentsTypes)
      components[t.name_hash] = new ComponentContainer(t.typeId, count);
  }
  ComponentContainer *Archetype::get_container(const char *name)
  {
    return components[HashedString(name)];
  }
  ComponentContainer *Archetype::get_container(string_hash name)
  {
    return components[name];
  }
  ComponentContainer::ComponentContainer(uint type, int count):
  data(calloc(count, types[type].sizeOf)), typeID(type), componentCount(count)
  {  }
  void* ComponentContainer::get_component(int i)
  {
    if (0 <= i && i < componentCount)
      return ((char*)data + i * types[typeID].sizeOf);
    return nullptr;
  }
  
  
  template<typename T>
  T* get_component(Entity entity, const char *name)
  {
    Archetype& archetype = archetypes[entity.archetype_index()];
    ComponentContainer *container = archetype.components[HashedString(name)];
    return container ? container->get_component(entity.array_index()) : nullptr;
  }

  template<typename T>
  uint type_index()
  {
    static uint index = globalTypeIndex++;
    return index;
  };


  template<typename T>
  TypeDescription get_type_description(const char *name)
  {
    TypeDescription type = {HashedString(name), type_index<T>()};
    if (globalTypeIndex != type.typeId)
    {
      types.emplace_back();
      types.back() = {std::string(name), type.name_hash, sizeof(T)};
    }
    return type;
  }
  
  SystemArchetypes register_system(const SystemDescription& descr)
  {
    SystemArchetypes sys_archetypes;
    for (Archetype *archetype: archetypes)
    {
      vector<ComponentContainer*> containers(descr.args.size());
      bool breaked = false;
      int i = 0;
      for(auto& arg : descr.args)
      {
        ComponentContainer* container = archetype->get_container(arg.descr.name_hash);
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
        sys_archetypes.archetypes.push_back(archetype);
    }
    return sys_archetypes;
  }
  void initialize_ecs()
  {
    
    ComponentTypes c1 = {{get_type_description<int>("a"), get_type_description<float>("b")}};
    ComponentTypes c2 = {{get_type_description<string>("s"), get_type_description<float>("f")}};
    archetypes.push_back(new Archetype(c1, 1));
    archetypes.push_back(new Archetype(c2, 2));
    
    //init archetypes??

  }
}

