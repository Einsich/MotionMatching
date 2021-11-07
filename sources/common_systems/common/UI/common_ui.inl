#include <ecs.h>
#include <application/time.h>
#include <imgui.h>
#include "editor_window.h"

void debug_show();
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor,ecs::SystemTag::Game) fps_ui(const EditorUI &ui)
{
  ImGui::Begin("fps", nullptr, ui.windowFlags | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("%.1f fps", Time::fps());
  ImGui::End();
}
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::GameEditor,ecs::SystemTag::Debug) debug_console_ui(const EditorUI &ui)
{
  ImGui::Begin("debug", nullptr, ui.windowFlags);
  debug_show();
  ImGui::End();
}