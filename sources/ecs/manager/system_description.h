#pragma once
#include "archetype.h"
#include "callable_description.h"

namespace ecs
{
  struct QueryDescription final : CallableDescription
  {
    QueryDescription(const char *name,
      ecs::vector<FunctionArgument> &&require_args,
      ecs::vector<FunctionArgument> &&require_not_args);
  };
  struct SystemDescription final : CallableDescription
  {
    void (*function)();
    const char *stage;
    bool isJob;
    SystemDescription(const char *name,
      ecs::vector<FunctionArgument> &&require_args,
      ecs::vector<FunctionArgument> &&require_not_args,
      ecs::vector<ecs::string> &&scenes,
      ecs::vector<ecs::string> &&before,
      ecs::vector<ecs::string> &&after,
      void (*function_pointer)(),
      const char * stage,
      ecs::vector<ecs::string> &&tags,
      bool is_job);
    void execute();
  };

  ecs::vector<SystemDescription *>& get_all_mutable_systems();
  ecs::vector<SystemDescription *>& get_all_systems();
  ecs::vector<QueryDescription*> &all_queries();

  struct SystemStageInterval
  {
    SystemDescription **begin, **end;

    SystemStageInterval(SystemDescription**begin, SystemDescription**end);
    SystemStageInterval():begin(nullptr), end(nullptr){}
    void perform_stage() const;
  };
  SystemStageInterval get_system_stage(const char *stage);
  void perform_stage(const char *stage);
}