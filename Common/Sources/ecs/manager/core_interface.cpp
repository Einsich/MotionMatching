#include "core_interface.h"
#include "../ecs_core.h"
namespace ecs
{
  uint type_sizeof(uint type)
  {
    return core().types[type].sizeOf;
  }
  CopyConstructor type_copy_constructor(uint type)
  {
    return core().types[type].copy_constructor;
  }
  Destructor type_destructor(uint type)
  {
    return core().types[type].destructor;
  }

  void add_system(SystemDescription *system_description)
  {
    core().systems.push_back(system_description);
  }
  void add_query(QueryDescription *query_description)
  {
    core().queries.push_back(query_description);
  }

  uint &global_type_index()
  {
    return core().globalTypeIndex;
  }


  std::unordered_map<uint, FullTypeDescription> &full_description()
  {
    return core().types;
  }
  void print_archetypes()
  {   
    printf("\n");
    for (const Archetype *archetype : core().archetypes)
    {
      printf("[A]{");
      for (const auto &descr : archetype->components)
      {
        auto &type = core().types[descr.second.typeHash];
        printf("  %s %s\n",type.type.c_str(), type.name.c_str());
      }

      printf("}\n");
    }
    std::fflush(stdout);
  }
}