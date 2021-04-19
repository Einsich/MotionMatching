#include "ecs/ecs.h"
#include "Time/time.h"
#include "imgui/imgui.h"

SYSTEM(ecs::SystemOrder::UI) fps_ui(float &fps)
{
  ImGui::Begin("fps");
  ImGui::Text("%.1f", fps = Time::fps());
  ImGui::End();
}