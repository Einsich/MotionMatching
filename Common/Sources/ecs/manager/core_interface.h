#pragma once
#include <sys/types.h>
#include <unordered_map>
namespace ecs
{
  typedef void *(*Constructor)(void*);
  typedef void *(*CopyConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
  uint type_sizeof(uint type);
  CopyConstructor type_copy_constructor(uint type);
  Destructor type_destructor(uint type);
  
  uint &global_type_index();
  template<typename T>
  uint type_index()
  {
    static uint index = global_type_index()++;
    return index;
  };

  class SystemDescription;
  void add_system(SystemDescription *system_description);
  class QueryDescription;
  void add_query(QueryDescription *query_description);
  
  void print_archetypes();
  class FullTypeDescription;

  std::unordered_map<uint, FullTypeDescription> &full_description();
}