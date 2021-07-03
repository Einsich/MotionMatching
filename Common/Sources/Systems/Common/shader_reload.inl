#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/Render/Shader/shader_factory.h"

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) reload_shaders(const KeyboardEvent &e)
{
  if (e.keycode == SDLK_F5)
  {
    compile_shaders();
    debug_log("shaders were recompiled");
  }
}