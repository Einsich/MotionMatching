#include "ecs_core.h"
#include "ecs_types.h"
#include "system_description.h"
#include "ecs/test_header.h"
namespace ecs
{
  struct Core
  {
    uint globalTypeIndex = 0;
    vector<FullTypeDescription> types;
    vector<Archetype*> archetypes;
    std::vector<SystemDescription*> systems;
    Core()
    {
    }
    ~Core()
    {
      for (Archetype *archetype : archetypes)
        archetype->~Archetype();
    }
  };
  static Core &core()
  {
    static Core *singleton = new Core();
    return *singleton;
  }

  static ComponentContainer *dummyContainer = new ComponentContainer();
  
  
  template<typename T>
  uint type_index()
  {
    static uint index = core().globalTypeIndex++;
    return index;
  };
  template<class T>
  void* template_constructor(void *memory)
  {
    return new (memory) T();
  }

  unsigned int Entity::archetype_index() const
  {
    return index >> 16;
  }
  unsigned int Entity::array_index() const
  {
    return index & index_mask;
  }

  FullTypeDescription::FullTypeDescription(string &&name, string_hash hash, uint size_of, void *(*constructor)(void*)):
  name(name), hash(hash), sizeOf(size_of), constructor(constructor)
  {
  }
  TypeDescription::TypeDescription(string_hash name_hash, uint typeId):
    name_hash(name_hash), typeId(typeId){}
  template<typename T>
  TypeDescription::TypeDescription(const char *name):
    name_hash(HashedString(name)), typeId(type_index<T>()){}
  
  string_hash TypeDescription::hash() const
  {
    return name_hash ^ (typeId * 16397816463u);
  }
  Archetype::Archetype(const ComponentTypes &types, int count):
    components(), componentCount(count)
  {
    for(const auto& t : types.componentsTypes)
    components.try_emplace(t.hash(), t.typeId, count);
  }
  ComponentContainer *Archetype::get_container(const TypeDescription &type)
  {
    auto it = components.find(type.hash());
    return it == components.end() ? dummyContainer : &it->second;
  }
  template<typename T>
  ComponentContainer *Archetype::get_container(const char *name)
  {
    auto it = components.find(TypeDescription::TypeDescription<T>(name).hash());
    return it == components.end() ? dummyContainer : &it->second;
  }
  ComponentContainer::ComponentContainer():
  data(nullptr), typeID(-1), componentCount(0)
  {  }
  ComponentContainer::ComponentContainer(uint type, int count):
  data(malloc(count * core().types[type].sizeOf)), typeID(type), componentCount(count)
  { 
    for (int i = 0; i < componentCount; ++i)
      core().types[typeID].constructor((char*)data + core().types[type].sizeOf * i);
  }
  void* ComponentContainer::get_component(int i)
  {
    if (0 <= i && i < componentCount)
      return ((char*)data + i * core().types[typeID].sizeOf);
    return nullptr;
  }
  ComponentContainer::~ComponentContainer()
  {
    free(data);
  }
  
  
  template<typename T>
  T* get_component(Entity entity, const char *name)
  {
    Archetype& archetype = core().archetypes[entity.archetype_index()];
    
    ComponentContainer *container = archetype.components[TypeDescription::TypeDescription<T>(name).hash()];
    return container ? container->get_component(entity.array_index()) : nullptr;
  }



  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(containers){}

  SystemDescription::SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)()):
    args(std::move(args)), archetypes(), function(function_pointer)
  {
    core().systems.push_back(this);
  }
  void SystemDescription::execute()
  {
    if (function)
      function();
  }
  template<typename T>
  TypeDescription get_type_description(const char *name)
  {
    uint globalTypeIndexCopy = core().globalTypeIndex;
    TypeDescription type = {HashedString(name), type_index<T>()};
    if (core().globalTypeIndex != globalTypeIndexCopy)
    {
      core().types.emplace_back(std::move(std::string(name)), type.name_hash, sizeof(T), template_constructor<T>);
    }
    return type;
  }
  
  void register_archetype(Archetype *archetype)
  {
    for (SystemDescription *system: core().systems)
    {
      std::vector<SystemCashedArchetype> &sys_archetypes = system->archetypes;
      vector<ComponentContainer*> containers(system->args.size());
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
    ComponentTypes c2 = {{get_type_description<string>("s"), get_type_description<float>("f")}};
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

