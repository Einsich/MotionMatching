#include <ecs/ecs.h>
#include <Engine/time.h>
#include <Engine/imgui/imgui.h>
#include "editor_window.h"
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor,ecs::SystemTag::Game) fps_ui(const EditorUI &ui)
{
  ImGui::Begin("fps", nullptr, ui.windowFlags | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("%.1f fps", Time::fps());
  ImGui::End();
}
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor,ecs::SystemTag::Game) debug_console_ui(const EditorUI &ui)
{
  ImGui::Begin("debug", nullptr, ui.windowFlags);
  debug_show();
  ImGui::End();
}