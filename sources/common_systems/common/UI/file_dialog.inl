#include <ecs.h>
#include <input.h>
#include <application/file_dialog.h>
#include <application/application_data.h>
#include <ecs_scene.h>
#include <editor/template.h>
#include "editor_window.h"

void open_scene(const filesystem::path &path, bool need_to_add);

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) open_dialog(EditorUI &ui)
{
  if (ImGui::BeginMenu("Editor"))
  {
    if (ImGui::Button("open scene"))
    {
      filesystem::path path;
      if (get_open_file(path, {"Scene", ".scene"}))
      {
        add_open_scene(path, false, true);
      }
    }
    if (ImGui::Button("create scene"))
    {
      filesystem::path path;
      if (get_save_file(path, ".scene"))
      {
        std::ofstream outfile(path, ios_base::binary);
        add_open_scene(path, true, true);
      }
    }
    if (ImGui::Button("save scene"))
    {
      Application::instance().scene->save_current_scene();
    }
    if (ImGui::Button("save templates"))
    {
      ecs::save_templates();
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