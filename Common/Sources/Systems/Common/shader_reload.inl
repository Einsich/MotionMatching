#include <ecs/ecs.h>
#include <Engine/Resources/resources.h>
#include <Engine/input.h>
#include <Engine/Render/Shader/shader_factory.h>
#include <Engine/Render/skybox.h>
#include <Engine/Render/direction_light.h>

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
EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_shaders(const KeyDownEvent<SDLK_F5> &)
{
  compile_shaders();
  foreach_resources<Material>([](Asset<Material> m){m->load("", false);});
  debug_log("shaders were recompiled");
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