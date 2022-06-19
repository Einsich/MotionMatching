#pragma once
#include "manager/component_instance.h"
#include "manager/component_container.h"
#include "manager/archetype.h"

namespace ecs
{
  struct Template
  {
    string name;
    vector<ComponentInstance> components;
    vector<ComponentContainer*> containers;
    Archetype *archetype;
    Template(const char *name, vector<ComponentInstance> &&components);
  };

  const Template* get_template(const char *name);
  Template *create_template(const char *name, vector<ComponentInstance> &&components);
  void invalidate_cached_archetype();
}
