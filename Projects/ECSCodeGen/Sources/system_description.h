#pragma once
#include "type_description.h"
#include "archetype.h"

namespace ecs
{
  struct FunctionArgument
  {
    TypeDescription descr;
    bool optional = false;
  };
  struct SystemCashedArchetype
  {
    Archetype *archetype;
    std::vector<ComponentContainer*> containers;
    SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers);
  };
  struct SystemDescription
  {
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    SystemDescription(const std::vector<FunctionArgument> &args, void (*function_pointer)());
    void execute();
  };
}