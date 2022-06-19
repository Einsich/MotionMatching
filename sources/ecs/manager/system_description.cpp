#include "system_description.h"
#include "../ecs_tag.h"
namespace ecs
{
  SystemCashedArchetype::SystemCashedArchetype(Archetype *archetype, ecs::vector<ComponentContainer*> &&containers):
    archetype(archetype), containers(std::move(containers)){}


  CallableDescription::CallableDescription(const char *name, 
    ecs::vector<FunctionArgument> &&require_args,
    ecs::vector<FunctionArgument> &&require_not_args,
    ecs::vector<ecs::string> &&scenes,
    ecs::vector<ecs::string> &&before,
    ecs::vector<ecs::string> &&after,
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
    ecs::vector<FunctionArgument> &&require_args,
    ecs::vector<FunctionArgument> &&require_not_args,
    ecs::vector<ecs::string> &&scenes, 
    ecs::vector<ecs::string> &&before, ecs::vector<ecs::string> &&after,
    void (*function_pointer)(), int stage, uint tags, bool is_job):
    CallableDescription(name, std::move(require_args), std::move(require_not_args),
      std::move(scenes), std::move(before), std::move(after), tags),
    function(function_pointer),
    stage(stage),
    isJob(is_job)
  {
    get_all_mutable_systems().push_back(this);
  }
  void SystemDescription::registration()
  {
  }
  void SystemDescription::execute()
  {
    function();
  }

  ecs::vector<QueryDescription*> &all_queries()
  {
    static ecs::vector<QueryDescription*> queries;
    return queries;
  }

  QueryDescription::QueryDescription(const char *name, 
    ecs::vector<FunctionArgument> &&require_args,
    ecs::vector<FunctionArgument> &&require_not_args):
    CallableDescription(name, std::move(require_args), std::move(require_not_args), {}, {}, {}, tags::all)
  {
    all_queries().push_back(this);
    isQuery = true;
  }
  void QueryDescription::registration()
  {
  }
  
  ecs::vector<SystemDescription *> &get_all_mutable_systems()
  {
    static ecs::vector<SystemDescription *> systems;
    return systems;
  }
  ecs::vector<SystemDescription *> &get_all_systems()
  {
    static ecs::vector<SystemDescription *> systems;
    return systems;
  }
}
