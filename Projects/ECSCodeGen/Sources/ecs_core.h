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
  };
  struct TypeDescription
  {
    string_hash name_hash;
    uint typeId;
  };

  class ComponentContainer
  {
    void* data;
  public:
    uint typeID;
    int componentCount;
    ComponentContainer() = delete;
    ComponentContainer(uint type, int count);
    void* get_component(int i);
  };

  struct ComponentTypes
  {
    vector<TypeDescription> componentsTypes;
  };
  
  class Archetype
  {
    unordered_map<string_hash, ComponentContainer*> components;
  public: 
    Archetype(const ComponentTypes &types, int count);
    ComponentContainer *get_container(const char *name);
    ComponentContainer *get_container(string_hash name);
  };
  struct SystemArchetypes
  {
    vector<Archetype*> archetypes;
  };

  struct FunctionArgument
  {
    TypeDescription descr;
    bool optional = false;
  };
  struct SystemDescription
  {
    std::vector<FunctionArgument> args;
    SystemArchetypes
  };

  template<typename T>
  T* get_component(Entity entity, const char *name);

  template<typename T>
  TypeDescription get_type_description(const char *name);

  void initialize_ecs();

  SystemArchetypes register_system(const SystemDescription& descr);

}
