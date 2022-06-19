#pragma once
#include "entity_pull.h"
#include "config/config.h"
namespace ecs
{
  class Archetype;
  struct EntityContainer
  {
    ecs::vector<Archetype*> archetypes;
    EntityPull entityPull;
    bool loaded;
  };
}