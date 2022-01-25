#include "system_description.h"
#include "core_interface.h"
#include "../system_tag.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(containers){}

  SystemDescription::SystemDescription(const char *name, std::vector<FunctionArgument> &&args, 
    void (*function_pointer)(), int order, uint tags, 
    std::vector<std::string> &&before, std::vector<std::string> &&after):
    QueryDescription(name, std::move(args), false),
    function(function_pointer),
    order(order),
    tags(tags),
    before(std::move(before)),
    after(std::move(after))
  {
    add_system(this);
  }
  void SystemDescription::execute()
  {
    function();
  }


  QueryDescription::QueryDescription(const char *name, std::vector<FunctionArgument> &&in_args, bool query):
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
}
