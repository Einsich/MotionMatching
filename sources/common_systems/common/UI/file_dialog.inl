#include <ecs.h>
#include <application/file_dialog.h>
#include "editor_window.h"

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) open_dialog(EditorUI &ui)
{
  if (ImGui::BeginMenu("Editor"))
  {
    if (ImGui::Button("open scene"))
    {
      filesystem::path path;
      if (get_open_file(path, {"Scene", ".blk"}))
      {
        ecs::create_scene(path.string());
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