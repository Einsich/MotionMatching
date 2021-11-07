#include "component_container.h"
#include "core_interface.h"
#include <stdlib.h>
namespace ecs
{

  ComponentContainer::ComponentContainer():
  data(), typeHash(-1), count(0)
  {  }
  ComponentContainer::ComponentContainer(const TypeInfo &type_info, string_hash type_name_hash, int capacity):
  data((capacity + binSize - 1) / binSize),
  typeHash(type_info.hashId),
  typeNameHash(type_name_hash),
  count(0),
  capacity(data.size() * binSize),
  sizeOf(type_info.sizeOf),
  constructor(type_info.constructor),
  copy_constructor(type_info.copy_constructor),
  destructor(type_info.destructor)
  { 
    for (uint i = 0; i < data.size(); ++i)
      data[i] = malloc(binSize * sizeOf);
  }
  ComponentContainer::~ComponentContainer()
  {
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

  void ComponentContainer::add_component(void *component_data)
  {
    void *dst = add_component();
    constructor(dst);
    copy_constructor(component_data, dst);
  }
  void* ComponentContainer::add_component()
  {
    if (count == capacity)
    {
      data.push_back(malloc(binSize * sizeOf));
      capacity += binSize;
    }
    int j = count / binSize;
    int i = count % binSize;
    void *dst = (char*)data[j] + sizeOf * i;
    count++;
    return dst;
  }
  void ComponentContainer::destroy_component(int i, bool with_swap)
  {
    count--;
    int j = count;
    void *removed = (char*)data[i / binSize] + sizeOf * (i % binSize);
    destructor(removed);
    if (with_swap && j != i)
    {
      void *copied = (char*)data[j / binSize] + sizeOf * (j % binSize);
      copy_constructor(copied, removed);
    }
  }
  
  void ComponentContainer::copy_components(const ComponentContainer &other)
  {
    assert(count == 0 && "Need empty container to copy to");
    while (other.count >= capacity)
    {
      data.push_back(malloc(binSize * sizeOf));
      capacity += binSize;
    }

    for (;count < other.count; ++count)
    {
      int j = count / binSize;
      int i = count % binSize;
      const void *scr = (char*)other.data[j] + sizeOf * i;
            void *dst = (char*)      data[j] + sizeOf * i;
      constructor(dst);
      copy_constructor(scr, dst);
    }
  }
}