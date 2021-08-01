#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/Render/Shader/shader_factory.h"
#include "Engine/Render/skybox.h"

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_shaders(const KeyDownEvent<SDLK_F5> &)
{
  compile_shaders();
  debug_log("shaders were recompiled");
}

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_sky_box(const ecs::OnEntityCreated &, SkyBox &skyBox)
{
  if (!skyBox.skyboxVAO.is_valid())
  {
    skyBox = SkyBox(skyBox.path);
  }
}