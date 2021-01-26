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
    unsigned int index;//archetype index + index in archetype

  };
  class TypeDescription
  {
  public:
    string name;
    int size, uniqueID;
    TypeDescription(const string& name, int size, int uniqueID)
    :name(name), size(size), uniqueID(uniqueID){}
  };
  class ComponentContainer
  {
    void* data;
    const TypeDescription& type;
    int componentCount;
    ComponentContainer(const TypeDescription& type, int count):
    data(malloc(count * type.size)), type(type), componentCount(count)
    {

    }
    void* get_component(int i)
    {
      if (0 <= i && i < componentCount)
        return ((char*)data + i * type.size);
      return nullptr;
    }
  };
  class Archetype
  {
    map<string, ComponentContainer> components;
    
  };
  template<typename T>
  T* get_component(Entity entity, const string& name);

}
