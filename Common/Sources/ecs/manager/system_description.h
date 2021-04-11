#pragma once
#include <iterator>
#include "archetype.h"

namespace ecs
{
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

  class SystemIterator;
  struct SystemDescription
  {
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)());
    void execute();
    SystemIterator begin();
    SystemIterator end();
  };
  
  class SystemIterator
  {
  public:
    SystemIterator(const SystemDescription &system, int archetype, int component);

    bool operator!=(SystemIterator const& other) const;
    void operator++();
    template<typename T>
    T *get_component(int ind)
    {
      return system.archetypes[archetypeIndex].containers[ind]->get_component<T>(componentIndex);
    }
  private:
    const SystemDescription &system;
    int archetypeIndex;
    int componentIndex;
  };
}