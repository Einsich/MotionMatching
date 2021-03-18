#pragma once
#include <vector>
#include <map>
#include <string>
#include <assert.h>
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
  class TypeDescription
  {
  public:
    string name;
    int size, uniqueID;
    TypeDescription(const char* name, unsigned long size, int uniqueID)
    :name(name), size(size), uniqueID(uniqueID){}
  };

  class ComponentContainer
  {
    void* data;
    const TypeDescription& type;
    int componentCount;
  public:
    ComponentContainer(const TypeDescription& type, int count);
    void* get_component(int i);
    const TypeDescription& get_description() const;
  };

  struct SystemArchetypes
  {
    vector<vector<ComponentContainer*>> archetypes;
  };
  class Archetype
  {
    map<string, ComponentContainer> components;
  public: 
    Archetype(vector<pair<string, pair<TypeDescription, int>>> types);
    ComponentContainer* get_container(const string& name);
  };
  template<typename T>
  T* get_component(Entity entity, const string& name);

  void initialize_ecs();

  SystemArchetypes register_system(const SystemDesription& descr);


}
