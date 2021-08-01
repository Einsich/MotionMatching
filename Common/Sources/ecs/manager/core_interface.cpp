
#include <iostream>
#include "core_interface.h"
#include "../ecs_core.h"
#include "entity_container.h"
namespace ecs
{
  uint32_t type_sizeof(uint32_t type)
  {
    return TypeInfo::types()[type].sizeOf;
  }
  CopyConstructor type_copy_constructor(uint32_t type)
  {
    return TypeInfo::types()[type].copy_constructor;
  }
  Destructor type_destructor(uint32_t type)
  {
    return TypeInfo::types()[type].destructor;
  }

  void add_system(SystemDescription *system_description)
  {
    core().systems.push_back(system_description);
  }
  void add_query(QueryDescription *query_description)
  {
    core().queries.push_back(query_description);
  }




  std::unordered_map<uint32_t, FullTypeDescription> &full_description()
  {
    return core().types;
  }
  void print_archetypes()
  {   
    printf("\n- - - - - \n");
    for (const Archetype *archetype : core().entityContainer->archetypes)
    {
      printf("{\n");
      for (const auto &descr : archetype->components)
      {
        auto &ecsType = core().types[descr.first];
        auto &cppType = TypeInfo::types()[descr.second.typeHash];
        printf("  %s %s\n",cppType.name.c_str(), ecsType.name.c_str());
      }

      printf("},\n");
    }
    std::fflush(stdout);
  }
}