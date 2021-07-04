#pragma once
#include <vector>
#include "entity_pull.h"
namespace ecs
{
  class Archetype;
  struct EntityContainer
  {
    std::vector<Archetype*> archetypes;
    EntityPull entityPull;
    void copy_from(EntityContainer *other);
  };
}