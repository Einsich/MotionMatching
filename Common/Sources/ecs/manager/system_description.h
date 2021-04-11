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

  class QueryIterator;

  struct QueryDescription
  {
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    QueryDescription(const std::vector<FunctionArgument> &args, bool query = true);
    QueryIterator begin();
    QueryIterator end();
  };
  struct SystemDescription : QueryDescription
  {
    void (*function)();
    SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)());
    void execute();
  };
  
  class QueryIterator
  {
  public:
    QueryIterator(const QueryDescription &query, int archetype, int component);

    bool operator!=(QueryIterator const& other) const;
    void operator++();
    template<typename T>
    T *get_component(int ind)
    {
      return query.archetypes[archetypeIndex].containers[ind]->get_component<T>(componentIndex);
    }
  private:
    const QueryDescription &query;
    int archetypeIndex;
    int componentIndex;
  };
}