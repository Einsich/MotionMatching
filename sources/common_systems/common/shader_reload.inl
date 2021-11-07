#include <ecs.h>
#include <resources/resources.h>
#include <input.h>
#include <render/shader/shader_factory.h>
#include <render/skybox.h>
#include <render/direction_light.h>

REGISTER_TYPE(SkyBox)

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
EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_shaders(const KeyDownEvent<SDLK_F5> &)
{
#ifndef RELEASE
  recompile_shaders();
  foreach_resources<Material>([](Asset<Material> m){m->load("", false);});
  QUERY()update_material([](Asset<Material> &material){if (material.is_copy()) material->load("", false);});
  debug_log("shaders were recompiled");
#endif
}

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) 
load_directional_light(const ecs::OnEntityCreated &, DirectionLight &directionalLight)
{
  directionalLight.normalizedLightDirection = normalize(directionalLight.lightDirection);
}
EVENT(ecs::SystemTag::Editor) 
reload_directional_light(const ecs::OnEntityEdited &, DirectionLight &directionalLight)
{
  directionalLight.normalizedLightDirection = normalize(directionalLight.lightDirection);
}