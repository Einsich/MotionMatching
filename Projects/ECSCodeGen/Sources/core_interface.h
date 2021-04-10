#pragma once
#include <sys/types.h>
#include <vector>
namespace ecs
{
  typedef void *(*constructor)(void*);
  uint type_sizeof(uint type);
  constructor type_constructor(uint type);

  uint &global_type_index();
  template<typename T>
  uint type_index()
  {
    static uint index = global_type_index()++;
    return index;
  };

  class SystemDescription;
  void add_system(SystemDescription *system_description);



  class FullTypeDescription;

  std::vector<FullTypeDescription> &full_description();
}