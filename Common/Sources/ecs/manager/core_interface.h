#pragma once
#include <stdint.h>
#include "component_function.h"
#include <unordered_map>
namespace ecs
{
  uint32_t type_sizeof(uint32_t type);
  CopyConstructor type_copy_constructor(uint32_t type);
  Destructor type_destructor(uint32_t type);


  struct SystemDescription;
  void add_system(SystemDescription *system_description);
  struct QueryDescription;
  void add_query(QueryDescription *query_description);
  
  void print_archetypes();
  struct FullTypeDescription;

  std::unordered_map<uint32_t, FullTypeDescription> &full_description();
}