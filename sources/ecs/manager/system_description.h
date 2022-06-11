#pragma once
#include "archetype.h"
#include "callable_description.h"

namespace ecs
{
  struct QueryDescription final : CallableDescription
  {
    QueryDescription(const char *name,
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args);
    void registration() override;
  };
  struct SystemDescription final : CallableDescription
  {
    void (*function)();
    int stage;
    bool isJob;
    SystemDescription(const char *name,
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      std::vector<std::string> &&before, std::vector<std::string> &&after,
      void (*function_pointer)(),
      int stage, uint tags, bool is_job);
    void execute();
    void registration() override;
  };
  
  std::vector<SystemDescription *>& get_all_mutable_systems();
  std::vector<SystemDescription *>& get_all_systems();
}