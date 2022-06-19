#pragma once
#include "manager/component_instance.h"
#include "manager/component_container.h"
#include "manager/archetype.h"

namespace ecs
{
  struct Template
  {
    std::string name;
    std::vector<ComponentInstance> components;
    std::vector<ComponentContainer*> containers;
    Archetype *archetype;
    Template(const char *name, std::vector<ComponentInstance> &&components);
  };

  const Template* get_template(const char *name);
  Template *create_template(const char *name, std::vector<ComponentInstance> &&components);
  void invalidate_cached_archetype();
}
