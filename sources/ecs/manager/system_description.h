#pragma once
#include <iterator>
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
    std::vector<std::string> before, after;
    uint tags;
    uint notSingletonArgsCount;
    bool isQuery;
    CallableDescription(const char *name,
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      std::vector<std::string> &&before,
      std::vector<std::string> &&after,
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
}