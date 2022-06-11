#include "core_interface.h"
#include "../ecs_core.h"
#include "entity_container.h"
namespace ecs
{

  std::unordered_map<uint, FullTypeDescription> &full_description()
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
        auto &cppType = *TypeInfo::types()[descr.second.typeHash];
        printf("  %s %s\n",cppType.name.c_str(), ecsType.name.c_str());
      }

      printf("},\n");
    }
    std::fflush(stdout);
  }
}