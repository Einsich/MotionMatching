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
    ecs::vector<ecs::string> &&tags):
    name(name),
    requireArgs(std::move(require_args)),
    requireNotArgs(std::move(require_not_args)),
    scenes(std::move(scenes)),
    before(std::move(before)),
    after(std::move(after)),
    tags(std::move(tags)),
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
    void (*function_pointer)(),
    const char * stage,
    ecs::vector<ecs::string> &&tags,
    bool is_job):
    CallableDescription(name, std::move(require_args), std::move(require_not_args),
      std::move(scenes), std::move(before), std::move(after), std::move(tags)),
    function(function_pointer),
    stage(stage),
    isJob(is_job)
  {
    get_all_mutable_systems().push_back(this);
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
    CallableDescription(name, std::move(require_args), std::move(require_not_args))
  {
    all_queries().push_back(this);
    isQuery = true;
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
  
  SystemStageInterval::SystemStageInterval(SystemDescription**begin, SystemDescription**end):
    begin(begin), end(end){}
  void SystemStageInterval::perform_stage() const
  {
    for (auto system = begin; system < end; ++system)
      (*system)->execute();
  }
  
  static bool lower_bound_cmp(const SystemDescription *a, const char *stage)
  {
    return strcmp(a->stage, stage) < 0;
  }
  static bool upper_bound_cmp(const char *stage, const SystemDescription *a)
  {
    return strcmp(a->stage, stage) > 0;
  }

  SystemStageInterval get_system_stage(const char *stage)
  {
    auto &systems = get_all_systems();
    return SystemStageInterval(
      eastl::lower_bound(systems.begin(), systems.end(), stage, lower_bound_cmp),
      eastl::upper_bound(systems.begin(), systems.end(), stage, upper_bound_cmp));
  }
  void perform_stage(const char *stage)
  {
    get_system_stage(stage).perform_stage();
  } 
}
