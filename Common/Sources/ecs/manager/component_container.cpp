#include "component_container.h"
#include "core_interface.h"
#include <stdlib.h>
namespace ecs
{

  ComponentContainer::ComponentContainer():
  data(), typeID(-1), count(0)
  {  }
  ComponentContainer::ComponentContainer(uint type, int capacity):
  data((capacity + binSize - 1) / binSize), typeID(type), count(0), capacity(data.size() * binSize)
  { 
    for (uint i = 0; i < data.size(); ++i)
      data[i] = malloc(binSize * type_sizeof(typeID));
  }
  ComponentContainer::~ComponentContainer()
  {
    Destructor destructor = type_destructor(typeID);
    uint sizeOf = type_sizeof(typeID);
    for (int i = 0, j = 0; i * binSize + j < count;)
    {
      void *removed = (char*)data[i] + sizeOf * j;
      destructor(removed);
      j++;
      if (j >= binSize)
      {
        j = 0;
        i++;
      }
    }
    for (uint i = 0; i < data.size(); ++i)
      free(data[i]);
  }

  void ComponentContainer::add_component(const ComponentInitializer &component)
  {
    if (count == capacity)
    {
      data.push_back(malloc(binSize * type_sizeof(typeID)));
      capacity += binSize;
    }
    CopyConstructor copyConstructor = type_copy_constructor(typeID);

    int j = count / binSize;
    int i = count % binSize;

    copyConstructor(component.data, (char*)data[j] + type_sizeof(typeID) * i);
    count++;
  }
  void ComponentContainer::destroy_component(int i)
  {
    count--;
    CopyConstructor copyConstructor = type_copy_constructor(typeID);
    Destructor destructor = type_destructor(typeID);
    int j = count;
    void *removed = (char*)data[i / binSize] + type_sizeof(typeID) * (i % binSize);
    destructor(removed);
    if (j != i)
    {
      void *copied = (char*)data[j / binSize] + type_sizeof(typeID) * (j % binSize);
      copyConstructor(copied, removed);
    }
  }
}