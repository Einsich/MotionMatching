#include "ecs/ecs.h"
#include "Engine/Render/global_uniform.h"
#include "Engine/Render/render.h"

EVENT(ecs::SystemTag::GameEditor) add_global_uniform(const ecs::OnSceneCreated &)
{
  add_global_uniform<GlobalRenderData>("GlobalRenderData", 0);
}