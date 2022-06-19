#include <ecs.h>
#include <application/file_dialog.h>
#include "editor_window.h"
#include <imgui.h>

SYSTEM(stage=ui_menu; scene=editor) open_dialog(EditorUI &ui)
{
  if (ImGui::BeginMenu("Editor"))
  {
    if (ImGui::Button("open scene"))
    {
      std::filesystem::path path;
      if (get_open_file(path, {"Scene", ".blk"}))
      {
        ecs::create_scene(path.string().c_str());
      }
    }
    constexpr uint lockFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    bool unlock = ui.windowFlags == 0;
    if (ImGui::Checkbox("Unlock panels", &unlock))
    {
      ui.windowFlags = unlock ? 0 : lockFlag;
    }

    ImGui::EndMenu();
  }
}