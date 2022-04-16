#pragma once
#include <iterator>
#include "../system_order.h"
#include "archetype.h"

namespace ecs
{
  struct FunctionArgument
  {
    string_hash descr;
    bool optional = false;
  };
  struct SystemCashedArchetype
  {
    Archetype *archetype;
    std::vector<ComponentContainer*> containers;
    SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers);
  };

  struct CallableDescription
  {
    std::string name;
    std::vector<FunctionArgument> requireArgs, requireNotArgs;
    std::vector<SystemCashedArchetype> archetypes;
    std::vector<std::string> scenes;
    uint tags;
    uint notSingletonArgsCount;
    CallableDescription(const char *name,
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      uint tags);
    virtual void registration() = 0;
  };
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
    std::vector<std::string> before, after;
    SystemDescription(const char *name,
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      void (*function_pointer)(),
      int stage, uint tags,
      std::vector<std::string> &&before, std::vector<std::string> &&after);
    void execute();
    void registration() override;
  };
}