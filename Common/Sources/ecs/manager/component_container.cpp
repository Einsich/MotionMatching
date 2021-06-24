#include "component_container.h"
#include "core_interface.h"
#include <stdlib.h>
namespace ecs
{

  ComponentContainer::ComponentContainer():
  data(), typeHash(-1), count(0)
  {  }
  ComponentContainer::ComponentContainer(string_hash type_hash, string_hash type_name_hash, int capacity, int sizeOf):
  data((capacity + binSize - 1) / binSize), typeHash(type_hash), typeNameHash(type_name_hash), count(0), capacity(data.size() * binSize), sizeOf(sizeOf)
  { 
    for (uint i = 0; i < data.size(); ++i)
      data[i] = malloc(binSize * type_sizeof(typeHash));
  }
  ComponentContainer::~ComponentContainer()
  {
    Destructor destructor = type_destructor(typeHash);
    uint sizeOf = type_sizeof(typeHash);
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
      data.push_back(malloc(binSize * type_sizeof(typeHash)));
      capacity += binSize;
    }
    CopyConstructor copyConstructor = type_copy_constructor(typeHash);

    int j = count / binSize;
    int i = count % binSize;

    copyConstructor(component.data, (char*)data[j] + type_sizeof(typeHash) * i);
    count++;
  }
  void ComponentContainer::destroy_component(int i, bool without_copy)
  {
    count--;
    CopyConstructor copyConstructor = type_copy_constructor(typeHash);
    Destructor destructor = type_destructor(typeHash);
    int j = count;
    void *removed = (char*)data[i / binSize] + type_sizeof(typeHash) * (i % binSize);
    destructor(removed);
    if (!without_copy && j != i)
    {
      void *copied = (char*)data[j / binSize] + type_sizeof(typeHash) * (j % binSize);
      copyConstructor(copied, removed);
    }
  }
}