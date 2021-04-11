#include "core_interface.h"
#include "../ecs_core.h"
namespace ecs
{
  uint type_sizeof(uint type)
  {
    return core().types[type].sizeOf;
  }
  Constructor type_constructor(uint type)
  {
    return core().types[type].constructor;
  }
  CopyConstructor type_copy_constructor(uint type)
  {
    return core().types[type].copy_constructor;
  }

  void add_system(SystemDescription *system_description)
  {
    core().systems.push_back(system_description);
  }
  uint &global_type_index()
  {
    return core().globalTypeIndex;
  }


  std::vector<FullTypeDescription> &full_description()
  {
    return core().types;
  }
}