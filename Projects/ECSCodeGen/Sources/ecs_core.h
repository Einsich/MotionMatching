#pragma once
#include <vector>
#include <map>
#include <string>
#include <assert.h>
#include <unordered_map>
#include "hash_string.h"
using namespace std;
namespace ecs
{
  class Entity
  {
  private:
    const static unsigned int index_mask = 0x0000ffff;
    unsigned int index;//archetype index + index in archetype
  public:
    unsigned int archetype_index() const;
    unsigned int array_index() const;

  };
  struct FullTypeDescription
  {
    string name; 
    string_hash hash;
    uint sizeOf;
    void *(*constructor)(void*);
    FullTypeDescription(string &&name, string_hash hash, uint sizeOf, void *(*constructor)(void*));
    ~FullTypeDescription() = default;
  };
  struct TypeDescription
  {
    string_hash name_hash;
    uint typeId;
    TypeDescription(string_hash name_hash, uint typeId);
    template<typename T>
    TypeDescription(const char *name);
    string_hash hash() const;
  };

  class ComponentContainer
  {
    void* data;
  public:
    uint typeID;
    int componentCount;
    ComponentContainer();
    ComponentContainer(uint type, int count);
    void* get_component(int i);
    template<typename T>
    T* get_component(int i)
    {
      if (0 <= i && i < componentCount)
        return ((T*)data + i);
      return nullptr;
    }
    ~ComponentContainer();
  };

  struct ComponentTypes
  {
    vector<TypeDescription> componentsTypes;
  };
  
  class Archetype
  {
    unordered_map<string_hash, ComponentContainer> components;
  public: 
    int componentCount;
    Archetype(const ComponentTypes &types, int count);
    ComponentContainer *get_container(const TypeDescription &type);
    template<typename T>
    ComponentContainer *get_container(const char *name);
    ~Archetype() = default;
  };

  struct FunctionArgument
  {
    TypeDescription descr;
    bool optional = false;
  };
  struct SystemCashedArchetype
  {
    Archetype *archetype;
    std::vector<ComponentContainer*> containers;
    SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers);
  };
  struct SystemDescription
  {
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)());
    void execute();
  };

  template<typename T>
  T* get_component(Entity entity, const char *name);

  template<typename T>
  TypeDescription get_type_description(const char *name);

  void initialize_ecs();

  void update_systems();

  void free_ecs();

  void register_system(std::vector<FunctionArgument> &&args);

}
