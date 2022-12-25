#include <ecs/ecs.h>
#include <resources/resources.h>
#include <input.h>
#include <render/shader/shader_factory.h>
#include <render/direction_light.h>
#include <render/material.h>
#include <ecs/editor_events.h>


template<typename T>
void foreach_resources(std::function<void(Asset<T>)> && pred)
{
  auto assets = Resources::instance().assets.find(nameOf<T>::value);
  if (assets != Resources::instance().assets.end())
  {
    for (auto & asset : assets->second.resources)
    {
      pred(asset.second);
    }
  }
}
template<typename Callable>
void update_material(Callable);
EVENT(tags=debug) reload_shaders(const KeyDownEvent<SDLK_F5> &)
{
  recompile_shaders();
  AssetStatus status;
  foreach_resources<Material>([&status](Asset<Material> m){m->load("", false, status);});
  QUERY()update_material([&status](Asset<Material> &material){if (material.is_copy()) material->load("", false, status);});
  debug_log("shaders were recompiled");
}

EVENT() 
load_directional_light(const ecs::OnEntityCreated &, DirectionLight &directionalLight)
{
  directionalLight.normalizedLightDirection = normalize(directionalLight.lightDirection);
}
EVENT(tags=editor) 
reload_directional_light(const OnEntityEdited &, DirectionLight &directionalLight)
{
  directionalLight.normalizedLightDirection = normalize(directionalLight.lightDirection);
}