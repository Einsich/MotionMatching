#pragma once
#include <sys/types.h>
namespace ecs
{
  class ComponentContainer
  {
    void* data;
  public:
    uint typeID;
    int componentCount;
    ComponentContainer();
    ComponentContainer(uint type, int count);
    template<typename T>
    T* get_component(int i)
    {
      if (0 <= i && i < componentCount)
        return ((T*)data + i);
      return nullptr;
    }
    ~ComponentContainer();
  };
}