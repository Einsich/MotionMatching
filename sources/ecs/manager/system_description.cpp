#include "system_description.h"
#include "core_interface.h"
#include "../ecs_tag.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(std::move(containers)){}


  CallableDescription::CallableDescription(const char *name, 
    std::vector<FunctionArgument> &&require_args,
    std::vector<FunctionArgument> &&require_not_args,
    std::vector<std::string> &&scenes,
    std::vector<std::string> &&before,
    std::vector<std::string> &&after,
    uint tags):
    name(name),
    requireArgs(std::move(require_args)),
    requireNotArgs(std::move(require_not_args)),
    scenes(std::move(scenes)),
    before(std::move(before)),
    after(std::move(after)),
    tags(tags),
    isQuery(false)
  {
    notSingletonArgsCount = 0;
    for (auto arg : requireArgs)
      if(arg.descr)
      {
        notSingletonArgsCount++;
      }
  }

  SystemDescription::SystemDescription(const char *name, 
    std::vector<FunctionArgument> &&require_args,
    std::vector<FunctionArgument> &&require_not_args,
    std::vector<std::string> &&scenes,
    void (*function_pointer)(), int stage, uint tags, 
    std::vector<std::string> &&before, std::vector<std::string> &&after):
    CallableDescription(name, std::move(require_args), std::move(require_not_args),
      std::move(scenes), std::move(before), std::move(after), tags),
    function(function_pointer),
    stage(stage)
  {
    add_callable(this);
  }
  void SystemDescription::registration()
  {
    add_system(this);
  }
  void SystemDescription::execute()
  {
    function();
  }


  QueryDescription::QueryDescription(const char *name, 
    std::vector<FunctionArgument> &&require_args,
    std::vector<FunctionArgument> &&require_not_args):
    CallableDescription(name, std::move(require_args), std::move(require_not_args), {}, {}, {}, tags::all)
  {
    add_callable(this);
    isQuery = true;
  }
  void QueryDescription::registration()
  {
    add_query(this);
  }
}
