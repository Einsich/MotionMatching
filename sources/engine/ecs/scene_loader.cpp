#include "template.h"
#include "manager/entity_id.h"
#include "data_block/data_block.h"
#include "common.h"
namespace ecs
{
  
  void init_components_from_blk(const DataBlock *tmpl, ecs::vector<ComponentInstance> &components);
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list);

  void load_scene(const DataBlock &scene)
  {
    for (uint i = 0, n = scene.blockCount(); i < n; i++)
    {
      const DataBlock *entity = scene.getBlock(i);
      if (entity->name() != "entity")
        continue;
      const std::string *templateName = entity->get<std::string>("_template");
      if (!templateName)
      {
        debug_error("need specify _template:t=\"template_name\"; in %d block", i);
        continue;
      }
      const Template *templateInst = get_template(templateName->c_str());
      if (!templateInst)
      {
        debug_error("Doesn't exist template \"%s\"", templateName->c_str());
        continue;
      }
      ComponentInitializerList overrides;
      init_components_from_blk(entity, overrides);
      create_entity(templateInst, std::move(overrides));
    }
  }
}