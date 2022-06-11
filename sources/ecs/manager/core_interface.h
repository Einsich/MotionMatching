#pragma once
#include "common.h"
#include <unordered_map>
namespace ecs
{

  struct CallableDescription;
  void add_callable(CallableDescription *system_description);
  struct SystemDescription;
  void add_system(SystemDescription *system_description);
  struct QueryDescription;
  void add_query(QueryDescription *query_description);
  
  void print_archetypes();
  struct FullTypeDescription;

  std::unordered_map<uint, FullTypeDescription> &full_description();
}