#include "system_description.h"
#include "core_interface.h"
#include "../system_tag.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(containers){}

  SystemDescription::SystemDescription(const char *name, const std::vector<FunctionArgument> &args, 
    void (*function_pointer)(), int order, uint tags):
    QueryDescription(name, args, false),
    function(function_pointer),
    order(order),
    tags(tags)
  {
    add_system(this);
  }
  void SystemDescription::execute()
  {
    function();
  }


  QueryDescription::QueryDescription(const char *name, const std::vector<FunctionArgument> &in_args, bool query):
    name(name), args(std::move(in_args)), archetypes()
  {
    if (query)
      add_query(this);
    withArgs = false;
    realArgs = 0;
    for (auto arg : args)
      if(arg.descr)
      {
        withArgs = true;
        realArgs++;
      }
  }
  SingleQueryDescription::SingleQueryDescription(const char *name, const std::vector<FunctionArgument> &in_args, bool query):
    name(name), args(std::move(in_args)), archetypes()
  {
    if (query)
      add_query((QueryDescription*)this);
    withArgs = false;
    realArgs = 0;
    for (auto arg : args)
      if(arg.descr)
      {
        withArgs = true;
        realArgs++;
      }
  }

  QueryIterator QueryDescription::begin() const
  {
    QueryIterator it(this, 0, 0);
    if (withArgs)
    {
      it.skip_empty_archetype();
    }
    else
      it.componentCount = 2;
    return it;
  }
  QueryIterator QueryDescription::end() const
  {
    return withArgs ? QueryIterator(this, archetypes.size(), 0) : QueryIterator(this, 0, 1);
  }


  QueryIterator::QueryIterator(const QueryDescription *query, int archetype, int component):
    query(query), dataArrays(query->realArgs), archetypeIndex(archetype), componentIndex(component),
    binIndex(component >> binPow), inBinIndex(component & binMask)
    { }


  bool QueryIterator::operator!=(QueryIterator const& other) const
  {
    return archetypeIndex != other.archetypeIndex || componentIndex != other.componentIndex;
  }
  void QueryIterator::operator++()
  {
    componentIndex++;
    inBinIndex++;
    if (componentIndex < componentCount)
    {
      bool nextBin = inBinIndex >= binSize;
      inBinIndex = nextBin ? 0 : inBinIndex;
      binIndex += nextBin ? 1 : 0;
    }
    else
    {
      componentIndex = binIndex = inBinIndex = 0;
      archetypeIndex++;
      skip_empty_archetype();
    }
  }

  void QueryIterator::skip_empty_archetype()
  {
    while (archetypeIndex < query->archetypes.size() && query->archetypes[archetypeIndex].archetype->count == 0)
    {
      archetypeIndex++;
    }
    set_archetype();
  }
  void QueryIterator::set_archetype()
  {
    if (archetypeIndex < query->archetypes.size())
    {
      archetype = &query->archetypes[archetypeIndex];
      for (uint i = 0, n = dataArrays.size(); i < n; i++)
      {
        auto *container = archetype->containers[i];
        if (container)
          dataArrays[i] = {&container->data, true};
        else
          dataArrays[i] = {nullptr, false};

      }
      componentCount = archetype->archetype->count;
    }
  }
}
