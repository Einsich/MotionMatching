#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/Render/Shader/shader_factory.h"
#include "Engine/Render/skybox.h"

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_shaders(const KeyDownEvent<SDLK_F5> &)
{
  compile_shaders();
  debug_log("shaders were recompiled");
}

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) load_sky_box(const ecs::OnEntityCreated &, SkyBox &skyBox)
{
  if (!skyBox.skyboxVAO.is_valid())
  {
    skyBox = SkyBox(skyBox.path);
  }
}
EVENT(ecs::SystemTag::Editor) reload_sky_box(const ecs::OnEntityEdited &, SkyBox &skyBox)
{
  SkyBox bufSky = SkyBox(skyBox.path);
  if (bufSky.skybox.width() > 0) 
    skyBox = bufSky;
}