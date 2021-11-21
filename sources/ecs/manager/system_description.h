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

  struct QueryDescription
  {
    std::string name;
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    bool withArgs;
    uint realArgs;
    QueryDescription(const char *name, const std::vector<FunctionArgument> &args, bool query = true);
  };
  struct SystemDescription : QueryDescription
  {
    void (*function)();
    int order;
    uint tags;
    SystemDescription(const char *name, const std::vector<FunctionArgument> &args, void (*function_pointer)(), int order, uint tags);
    void execute();
  };
}