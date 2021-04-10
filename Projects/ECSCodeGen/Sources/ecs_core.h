#pragma once
#include <vector>
#include "string_hash.h"
#include "system_description.h"
namespace ecs
{
  struct Core
  {
    uint globalTypeIndex = 0;
    std::vector<FullTypeDescription> types;
    std::vector<Archetype*> archetypes;
    std::vector<SystemDescription*> systems;
    Core() = default;
    ~Core();
  };
  Core &core();
  class Entity
  {
  private:
    const static unsigned int index_mask = 0x0000ffff;
    unsigned int index;//archetype index + index in archetype
  public:
    unsigned int archetype_index() const;
    unsigned int array_index() const;

  };

  template<typename T>
  T* get_component(Entity entity, const char *name);


  void initialize_ecs();

  void update_systems();

  void free_ecs();


}
