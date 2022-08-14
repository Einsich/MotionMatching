#pragma once
#include "component_container.h"
namespace ecs
{
  class Archetype;
  struct FunctionArgument
  {
    uint typeHash;
    uint nameHash;
    bool optional = false;
  };
  struct SystemCashedArchetype
  {
    Archetype *archetype;
    ecs::vector<ComponentContainer*> containers;
    SystemCashedArchetype(Archetype *archetype, ecs::vector<ComponentContainer*> &&containers);
  };

  struct CallableDescription
  {
    ecs::string name;
    ecs::vector<FunctionArgument> requireArgs, requireNotArgs;
    ecs::vector<SystemCashedArchetype> archetypes;
    ecs::vector<ecs::string> before, after;
    ecs::vector<ecs::string> tags;
    uint notSingletonArgsCount;
    bool isQuery;
    CallableDescription(const char *name,
      ecs::vector<FunctionArgument> &&require_args,
      ecs::vector<FunctionArgument> &&require_not_args,
      ecs::vector<ecs::string> &&before={},
      ecs::vector<ecs::string> &&after={},
      ecs::vector<ecs::string> &&tags={});
    CallableDescription(const CallableDescription &) = delete;
    CallableDescription(CallableDescription &&) = delete;
    CallableDescription& operator= (const CallableDescription &) = delete;
    CallableDescription& operator= (CallableDescription &&) = delete;
  };
}