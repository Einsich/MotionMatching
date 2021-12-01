#pragma once
#include "../manager/component_instance.h"
#include "../manager/component_container.h"
#include "../manager/archetype.h"
#include "data_block/data_block.h"

namespace ecs
{
  struct BlkTemplate
  {
    string name;
    vector<ComponentInstance> components;
    mutable vector<ComponentContainer*> containers;
    mutable Archetype *archetype = nullptr;
  };

  const BlkTemplate* get_template(const char *name);
}
