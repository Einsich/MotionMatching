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
}