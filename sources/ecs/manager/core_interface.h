#pragma once
#include "common.h"
#include <unordered_map>
namespace ecs
{

  struct CallableDescription;
  void add_callable(CallableDescription *system_description);
  struct SystemDescription;
  void add_system(SystemDescription *system_description);
  
  void print_archetypes();
}