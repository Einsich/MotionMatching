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
    void registration() override;
  };
  struct SystemDescription final : CallableDescription
  {
    void (*function)();
    int stage;
    bool isJob;
    SystemDescription(const char *name,
      ecs::vector<FunctionArgument> &&require_args,
      ecs::vector<FunctionArgument> &&require_not_args,
      ecs::vector<ecs::string> &&scenes,
      ecs::vector<ecs::string> &&before, ecs::vector<ecs::string> &&after,
      void (*function_pointer)(),
      int stage, uint tags, bool is_job);
    void execute();
    void registration() override;
  };
  
  ecs::vector<SystemDescription *>& get_all_mutable_systems();
  ecs::vector<SystemDescription *>& get_all_systems();
}