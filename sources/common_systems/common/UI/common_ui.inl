#include <ecs.h>
#include <application/time.h>
#include <imgui.h>
#include "editor_window.h"

void debug_show();
SYSTEM(stage=ui) fps_ui(const EditorUI &ui)
{
  ImGui::Begin("fps", nullptr, ui.windowFlags | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("%.1f fps", Time::fps());
  ImGui::End();
}
SYSTEM(stage=ui;tags=debug) debug_console_ui(const EditorUI &ui)
{
  ImGui::Begin("debug", nullptr, ui.windowFlags);
  debug_show();
  ImGui::End();
}