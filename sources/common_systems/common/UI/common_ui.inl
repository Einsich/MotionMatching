#include <ecs/ecs.h>
#include <application/time.h>
#include <imgui.h>
#include "editor_window.h"

#include <ecs/singleton.h>

ECS_REGISTER_SINGLETON(EditorUI)

void debug_show();
SYSTEM(stage=imgui_render) fps_ui(const EditorUI &ui)
{
  ImGui::Begin("fps", nullptr, ui.windowFlags | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("%.1f fps", Time::fps());
  ImGui::End();
}
SYSTEM(tags=debug; stage=imgui_render) debug_console_ui(const EditorUI &ui)
{
  ImGui::Begin("debug", nullptr, ui.windowFlags);
  debug_show();
  ImGui::End();
}