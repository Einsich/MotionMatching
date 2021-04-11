#include "system_description.h"
#include "core_interface.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(containers){}

  SystemDescription::SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)(), SystemOrder order):
    QueryDescription(args, false),
    function(function_pointer),
    order((int)order)
  {
    add_system(this);
  }
  void SystemDescription::execute()
  {
    function();
  }


  QueryDescription::QueryDescription(const std::vector<FunctionArgument> &args, bool query):
    args(std::move(args)), archetypes()
  {
    if (query)
      add_query(this);
  }
  SingleQueryDescription::SingleQueryDescription(const std::vector<FunctionArgument> &args, bool query):
    args(std::move(args)), archetypes()
  {
    if (query)
      add_query((QueryDescription*)this);
  }

  QueryIterator QueryDescription::begin()
  {
    QueryIterator it(this, 0, 0);
    it.skip_empty_archetype();
    return it;
  }
  QueryIterator QueryDescription::end()
  {
    return QueryIterator(this, archetypes.size(), 0);
  }

  QueryIterator::QueryIterator():
  query(nullptr), archetypeIndex(-1), componentIndex(0){}

  QueryIterator::QueryIterator(const QueryDescription *query, int archetype, int component):
    query(query), archetypeIndex(archetype), componentIndex(component){}


  bool QueryIterator::operator!=(QueryIterator const& other) const
  {
    return archetypeIndex != other.archetypeIndex || componentIndex != other.componentIndex;
  }
  void QueryIterator::operator++()
  {
    componentIndex++;
    skip_empty_archetype();
  }

  void QueryIterator::skip_empty_archetype()
  {
    while ((uint)archetypeIndex < query->archetypes.size() && query->archetypes[archetypeIndex].archetype->count <= componentIndex)
    {
      componentIndex = 0;
      archetypeIndex++;
    }
  }
}