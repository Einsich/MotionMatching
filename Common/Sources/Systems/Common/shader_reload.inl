#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/Render/Shader/shader_factory.h"

EVENT() reload_shaders(const KeyboardEvent &e, const std::string &project)
{
  if (e.keycode == SDLK_F5)
  {
    compile_shaders();
    debug_log("shaders were recompiled");
  }
}