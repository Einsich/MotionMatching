#include "system_description.h"
#include "core_interface.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(containers){}

  SystemDescription::SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)()):
    args(std::move(args)), archetypes(), function(function_pointer)
  {
    add_system(this);
  }
  void SystemDescription::execute()
  {
    function();
  }

  SystemIterator SystemDescription::begin()
  {
    return SystemIterator(*this, 0, 0);
  }
  SystemIterator SystemDescription::end()
  {
    return SystemIterator(*this, archetypes.size(), 0);
  }

  SystemIterator::SystemIterator(const SystemDescription &system, int archetype, int component):
    system(system), archetypeIndex(archetype), componentIndex(component){}

  bool SystemIterator::operator!=(SystemIterator const& other) const
  {
    return archetypeIndex != other.archetypeIndex || componentIndex != other.componentIndex;
  }
  void SystemIterator::operator++()
  {
    componentIndex++;
    if (system.archetypes[archetypeIndex].archetype->componentCount <= componentIndex)
    {
      componentIndex = 0;
      archetypeIndex++;
    }
  }
}