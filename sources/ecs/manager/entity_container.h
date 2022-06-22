#pragma once
#include "entity_pull.h"
#include "config/config.h"
#include <memory>
namespace ecs
{
  class Archetype;
  struct EntityContainer
  {
    ecs::vector<std::unique_ptr<Archetype>> archetypes;
    EntityPull entityPull;
    bool loaded;
  };
}