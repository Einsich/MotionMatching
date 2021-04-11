#include "entity_id.h"
#include <assert.h>
#include "common.h"
namespace ecs
{
  constexpr uint index_mask = 0x000fffff;
  constexpr uint index_shift = 20u;
  constexpr uint bad_eid = 0xffffffff;
  EntityId::EntityId():
    eid(nullptr){}
    EntityId::EntityId(uint *id):
    eid(id){}
  EntityId::EntityId(uint *id, uint archetype, uint index):
    eid(id)
  {
    migrate(archetype, index);
  }
  EntityId::EntityId(const EntityId &id):
    eid(id.eid){}
  EntityId::EntityId(EntityId &&id):
    eid(id.eid)
  {
    id.eid = nullptr;
  }
  bool EntityId::valid() const
  {
    return eid != nullptr && *eid != bad_eid;
  }
  EntityId::operator bool() const
  {
    return valid();
  }
  EntityId &EntityId::operator= (const EntityId &id)
  {
    eid = id.eid;
    return *this;
  }
  bool EntityId::operator== (const EntityId &id)
  {
    return eid == id.eid;
  }
  bool EntityId::operator!= (const EntityId &id)
  {
    return !(*this == id);
  }
  void EntityId::migrate(uint archetype, uint index)
  {
    assert((index & index_mask) == index && "There are a lot of entities in archetype");
    assert(((archetype << index_shift) >> index_shift) == archetype && "Not enought archetypes bits");

    *eid = index | (archetype << index_shift);
  }
  int EntityId::archetype_index() const
  {
    return valid() ? *eid >> index_shift : -1;
  }
  uint EntityId::array_index() const
  {
    return valid() ? *eid & index_mask : 0;
  }


  constexpr uint entityBinSize = 200;

  EntityContainerIterator::EntityContainerIterator(const EntityContainer &container, int bin, int index):
  container(container), bin(bin), index(index){}

  bool EntityContainerIterator::operator!=(const EntityContainerIterator &other) const
  {
    return bin != other.bin || index != other.index;
  }
  void EntityContainerIterator::operator++()
  {
    index++;
    if (index == entityBinSize)
    {
      index = 0;
      bin++;
    }
  }
  EntityId EntityContainerIterator::eid() const
  {
    return EntityId(&container.entities[bin][index]);
  }




  EntityContainer::EntityContainer():
  entities(), totalCount(0), destroyCount(0), entityCapacity(0)
  {

  }
  EntityContainer::~EntityContainer()
  {
    for (uint *bins : entities)
      free(bins);
  }
  uint *EntityContainer::get_entity()
  {
    if (totalCount == entityCapacity)
    {
      entities.push_back((uint*)malloc(entityBinSize * sizeof(uint)));
      entityCapacity += entityBinSize;
    }
    uint *eid = entities[totalCount / entityBinSize] + (totalCount % entityBinSize);
    return eid;
  }
  EntityId EntityContainer::create_entity(uint archetype, uint index)
  {
    uint *entity = get_entity();
    totalCount++;
    return EntityId(entity, archetype, index);
  }
  void EntityContainer::destroy_entity(const EntityId &eid)
  {
    if (eid)
    {
      *eid.eid = bad_eid;
      destroyCount++;
    }
  }
  EntityContainerIterator EntityContainer::begin() const
  {
    return EntityContainerIterator(*this, 0, 0);
  }
  EntityContainerIterator EntityContainer::end() const
  {
    return EntityContainerIterator(*this, totalCount / entityBinSize, totalCount % entityBinSize);
  }
}