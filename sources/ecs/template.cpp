#include "template.h"
#include "ecs_core.h"
#include "manager/component_container.h"
#include "manager/archetype.h"
#include "manager/entity_container.h"
#include <map>
#include <memory>

namespace ecs
{
  Template::Template(const char *name, vector<ComponentInstance> &&components):
    name(name), components(std::move(components)), containers(), archetype(nullptr)
  {}
  
  Archetype *add_archetype(const Template &tmpl);

  struct TemplateManager
  {
    std::map<string, std::shared_ptr<Template>> templates;
    Template *find(const char *name)
    {
      auto it = templates.find(name);
      return it != templates.end() ? it->second.get() : nullptr;
    }
  };

  static TemplateManager &get_template_manager() 
  {
    static TemplateManager manager;
    return manager;
  }

  void update_template_cache(Template &tmpl)
  {
    for (Archetype *archetype : core().entityContainer->archetypes)
    {
      if (archetype->in_archetype(tmpl.components))
      {
        tmpl.archetype = archetype;
        break;
      }
    }
    if (!tmpl.archetype)
    {
      tmpl.archetype = add_archetype(tmpl);
    }
    size_t count = tmpl.components.size();
    tmpl.containers.reserve(count);
    auto &componentMap = tmpl.archetype->components; 
    for (const ComponentInstance &instance : tmpl.components)
    {
      tmpl.containers.emplace_back(&componentMap[instance.typeNameHash]);
    }
  }

  const Template* get_template(const char *name)
  {
    Template* tmpl = get_template_manager().find(name);

    return tmpl;
  }
  
  Template *create_template(const char *name, vector<ComponentInstance> &&components)
  {
    auto it = get_template_manager().templates.try_emplace(name, std::make_shared<Template>(name, std::move(components)));
    if (it.second)
    {
      return it.first->second.get();
    }
    else
    {
      return nullptr;
    }
  }

  void invalidate_cached_archetype()
  {
    for (auto &[name, t] : get_template_manager().templates)
    {
      t->archetype = nullptr;
      t->containers.clear();
    }
  }
}