#pragma once
#include "manager/component_instance.h"
#include "manager/component_container.h"
#include "manager/archetype.h"

namespace ecs
{
  struct Template
  {
    ecs::string name;
    ecs::vector<ComponentInstance> components;
    ecs::vector<ComponentContainer*> containers;
    Archetype *archetype;
    Template(const char *name, ecs::vector<ComponentInstance> &&components);
  };

  const Template* get_template(const char *name);
  Template *create_template(const char *name, ecs::vector<ComponentInstance> &&components);
  void invalidate_cached_archetype();
}
