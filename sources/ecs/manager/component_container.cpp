#include "component_container.h"
#include <stdlib.h>
namespace ecs
{

  ComponentContainer::ComponentContainer():
  data(), typeHash(-1), count(0)
  { }
  ComponentContainer::ComponentContainer(const TypeInfo &type_info, uint type_name_hash, int capacity):
  data((capacity + binSize - 1) >> binPow),
  typeHash(type_info.hashId),
  typeNameHash(type_name_hash),
  count(0),
  capacity(data.size() << binPow),
  rtti(type_info.rtti)
  { 
    for (uint i = 0; i < data.size(); ++i)
      data[i] = malloc(rtti.sizeOf << binPow);
  }
  ComponentContainer::~ComponentContainer()
  {
    for (uint i = 0, j = 0; (i << binPow) + j < count;)
    {
      void *removed = (char*)data[i] + rtti.sizeOf * j;
      rtti.destructor(removed);
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

  void ComponentContainer::add_component(const void *component_data)
  {
    void *dst = add_component();
    rtti.constructor(dst);
    rtti.copy_constructor(component_data, dst);
  }
  void ComponentContainer::add_component(void *component_data)
  {
    void *dst = add_component();
    rtti.constructor(dst);
    rtti.move_constructor(component_data, dst);
  }
  void* ComponentContainer::add_component()
  {
    if (count == capacity)
    {
      data.push_back(malloc(rtti.sizeOf << binPow));
      capacity += binSize;
    }
    int j = count >> binPow;
    int i = count & binMask;
    void *dst = (char*)data[j] + rtti.sizeOf * i;
    count++;
    return dst;
  }
  void ComponentContainer::destroy_component(int i, bool with_swap)
  {
    count--;
    int j = count;
    void *removed = (char*)data[i >> binPow] + rtti.sizeOf * (i & binMask);
    rtti.destructor(removed);
    if (with_swap && j != i)
    {
      void *copied = (char*)data[j >> binPow] + rtti.sizeOf * (j & binMask);
      rtti.copy_constructor(copied, removed);
    }
  }
  
  void ComponentContainer::copy_components(const ComponentContainer &other)
  {
    assert(count == 0 && "Need empty container to copy to");
    while (other.count >= capacity)
    {
      data.push_back(malloc(rtti.sizeOf << binPow));
      capacity += binSize;
    }

    for (;count < other.count; ++count)
    {
      int j = count >> binPow;
      int i = count & binMask;
      const void *scr = (char*)other.data[j] + rtti.sizeOf * i;
            void *dst = (char*)      data[j] + rtti.sizeOf * i;
      rtti.constructor(dst);
      rtti.copy_constructor(scr, dst);
    }
  }
}