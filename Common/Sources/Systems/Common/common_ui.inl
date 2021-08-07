#include "ecs/ecs.h"
#include "Engine/time.h"
#include "Engine/imgui/imgui.h"
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor,ecs::SystemTag::Game) fps_ui()
{
  ImGui::Begin("fps");
  ImGui::Text("%.1f", Time::fps());
  ImGui::End();
}
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor,ecs::SystemTag::Game) debug_console_ui()
{
  ImGui::Begin("debug");
  debug_show();
  ImGui::End();
}