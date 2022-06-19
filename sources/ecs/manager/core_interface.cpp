#include "core_interface.h"
#include "../ecs_core.h"
#include "entity_container.h"
namespace ecs
{
  void print_archetypes()
  {   
    printf("\n- - - - - \n");
    for (const Archetype *archetype : core().entityContainer->archetypes)
    {
      printf("{\n");
      for (const auto &[name, typeInfo, components, hash] : archetype->typeDescriptions)
      {
        printf("  %s %s\n", typeInfo->name.c_str(), name);
      }
      printf("},\n");
    }
    std::fflush(stdout);
  }
}