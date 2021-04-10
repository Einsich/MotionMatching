#include "component_container.h"
#include "core_interface.h"
#include <stdlib.h>
namespace ecs
{

  ComponentContainer::ComponentContainer():
  data(nullptr), typeID(-1), componentCount(0)
  {  }
  ComponentContainer::ComponentContainer(uint type, int count):
  data(malloc(count * type_sizeof(type))), typeID(type), componentCount(count)
  { 
    constructor componentConstructor = type_constructor(type);
    for (int i = 0; i < componentCount; ++i)
      componentConstructor((char*)data + type_sizeof(type) * i);
  }
  ComponentContainer::~ComponentContainer()
  {
    free(data);
  }
}