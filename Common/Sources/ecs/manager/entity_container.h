#pragma once
#include <vector>
#include "entity_pull.h"
#include "Serialization/iserializable.h"
namespace ecs
{
  class Archetype;
  struct EntityContainer : ISerializable
  {
    std::vector<Archetype*> archetypes;
    EntityPull entityPull;
    bool dontAddable;
    virtual size_t serialize(std::ostream& os) const override;
    virtual size_t deserialize(std::istream& is) override;
  };
}