#include <ecs/ecs.h>
#include <application/file_dialog.h>
#include <application/application.h>
#include "editor_window.h"
#include <imgui.h>
#include <ecs/imgui.h>

EVENT(tags=editor) open_dialog(const ImguiMenuRender&, EditorUI &ui)
{
  if (ImGui::BeginMenu("Editor"))
  {
    if (ImGui::Button("open scene"))
    {
      std::filesystem::path path;
      if (get_open_file(path, {"Scene", ".blk"}))
      {
        create_scene(path.string().c_str());
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