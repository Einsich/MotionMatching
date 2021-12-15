#include "blk_template.h"

namespace ecs
{
  
  void init_components_from_blk(const DataBlock *tmpl, vector<ComponentInstance> &components);
  EntityId create_entity(const BlkTemplate *temp, ComponentInitializerList &&list);

  void load_scene(const string &path)
  {
    DataBlock scene(path);
    for (uint i = 0, n = scene.blockCount(); i < n; i++)
    {
      const DataBlock *entity = scene.getBlock(i);
      if (entity->name() != "entity")
        continue;
      const string *templateName = entity->get<string>("_template");
      if (!templateName)
      {
        debug_error("need specify _template:t=\"template_name\"; in %d block", i);
        continue;
      }
      const BlkTemplate *templateInst = get_template(templateName->c_str());
      if (!templateInst)
      {
        debug_error("Doesn't exist template \"%s\"", templateName->c_str());
        continue;
      }
      ComponentInitializerList overrides;
      init_components_from_blk(entity, overrides.components);
      create_entity(templateInst, std::move(overrides));
    }
  }
}