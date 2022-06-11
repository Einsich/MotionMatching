#pragma once
#include "component_container.h"
namespace ecs
{
  class Archetype;
  struct FunctionArgument
  {
    uint descr;
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
    CallableDescription(const CallableDescription &) = delete;
    CallableDescription(CallableDescription &&) = delete;
    CallableDescription& operator= (const CallableDescription &) = delete;
    CallableDescription& operator= (CallableDescription &&) = delete;
  };
}