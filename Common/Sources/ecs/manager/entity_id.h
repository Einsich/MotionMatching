#pragma once
#include <stdlib.h>
#include <vector>
namespace ecs
{
  class EntityId
  {
  private:
    friend class EntityContainer;
    uint *eid;//archetype index + index in archetype
    bool valid() const;
  public:
    EntityId();
    EntityId(uint *id, uint archetype, uint index);
    void migrate(uint archetype, uint index);
    EntityId(const EntityId &id);
    EntityId(EntityId &&id);
    int archetype_index() const;
    uint array_index() const;
    operator bool() const;
    EntityId &operator= (const EntityId &id);
    bool operator== (const EntityId &id);
    bool operator!= (const EntityId &id);

  };
  class EntityContainer
  {
    friend class EntityId;
    std::vector<uint*> entities;
    int aliveCount, totalCount, destroyCount, entityCapacity;
    uint *get_entity();
  public:
    EntityContainer();
    ~EntityContainer();
    EntityId create_entity(uint archetype, uint index);
    void destroy_entity(const EntityId &eid);
  };
}