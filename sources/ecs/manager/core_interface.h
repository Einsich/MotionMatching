#pragma once
#include "common.h"
#include "data_block/data_block.h"
#include "component_function.h"
#include <unordered_map>
namespace ecs
{
  uint type_sizeof(uint type);
  Constructor copy_constructor(uint type);
  CopyConstructor type_copy_constructor(uint type);
  Destructor type_destructor(uint type);


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