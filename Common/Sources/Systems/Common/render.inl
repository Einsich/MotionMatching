#include "ecs/ecs.h"
#include "Engine/Render/global_uniform.h"
#include "Engine/Render/render.h"
#include "Engine/Render/mesh.h"
#include "Engine/Render/material.h"
#include "Engine/Render/Texture/textures.h"
#include "Engine/Resources/resources.h"
#include <Engine/imgui/imgui.h>

REGISTER_TYPE_NAMED(Asset<Mesh>, mesh)
REGISTER_TYPE_NAMED(Asset<Texture2D>, Texture2D)
REGISTER_TYPE_NAMED(Asset<Material>, material)

EVENT(ecs::SystemTag::GameEditor) add_global_uniform(const ecs::OnSceneCreated &)
{
  add_uniform_buffer<GlobalRenderData>("GlobalRenderData", 0);
  add_storage_buffer("InstanceData", 0, 1);
}

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) render_submenu(EditorRenderSettings &settings)
{
  if (ImGui::BeginMenu("Render"))
  {
    ImGui::Checkbox("wire frame", &settings.wire_frame);
    ImGui::EndMenu();
  }
}