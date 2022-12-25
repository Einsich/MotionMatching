#include <ecs/ecs.h>
#include "data_block/data_block.h"
#include "common.h"
namespace ecs_ex
{
  void init_components_from_blk(const DataBlock *tmpl, ecs::vector<ecs::ComponentPrefab> &components);
}

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
    ecs::prefab_id prefabId = ecs::get_prefab_id(templateName->c_str());
    if (prefabId == ecs::invalidPrefabId)
    {
      debug_error("Doesn't exist template \"%s\"", templateName->c_str());
      continue;
    }
    ecs::vector<ecs::ComponentPrefab> overrides;
    ecs_ex::init_components_from_blk(entity, overrides);
    ecs::create_entity(prefabId, std::move(overrides));
  }
}