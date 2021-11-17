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
    for (auto arg : args)
      if(arg.descr)
      {
        withArgs = true;
        break;
      }
  }
  SingleQueryDescription::SingleQueryDescription(const char *name, const std::vector<FunctionArgument> &in_args, bool query):
    name(name), args(std::move(in_args)), archetypes()
  {
    if (query)
      add_query((QueryDescription*)this);
    withArgs = false;
    for (auto arg : args)
      if(arg.descr)
      {
        withArgs = true;
        break;
      }
  }

  QueryIterator QueryDescription::begin()
  {
    QueryIterator it(this, 0, 0);
    if (withArgs)
    {
      it.skip_empty_archetype();
    }
    return it;
  }
  QueryIterator QueryDescription::end()
  {
    return withArgs ? QueryIterator(this, archetypes.size(), 0) : QueryIterator(this, 0, 1);
  }


  QueryIterator::QueryIterator(const QueryDescription *query, int archetype, int component):
    query(query), archetypeIndex(archetype), componentIndex(component),
    binIndex(component >> binPow), inBinIndex(component & binMask) {}


  bool QueryIterator::operator!=(QueryIterator const& other) const
  {
    return archetypeIndex != other.archetypeIndex || componentIndex != other.componentIndex;
  }
  void QueryIterator::operator++()
  {
    componentIndex++;
    inBinIndex++;
    if (query->withArgs)
    {
      if ((uint)archetype->archetype->count <= componentIndex)
      {
        componentIndex = binIndex = inBinIndex = 0;
        archetypeIndex++;
        skip_empty_archetype();
      }
      else if (inBinIndex >= binSize)
      {
        inBinIndex = 0;
        binIndex++;
      }
    }
  }

  void QueryIterator::skip_empty_archetype()
  {
    while (archetypeIndex < query->archetypes.size() && query->archetypes[archetypeIndex].archetype->count == 0)
    {
      archetypeIndex++;
    }
    if (archetypeIndex < query->archetypes.size())
      archetype = &query->archetypes[archetypeIndex];
  }
}