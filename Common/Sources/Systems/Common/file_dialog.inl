#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/file_dialog.h"

void open_scene(const filesystem::path &path, bool need_to_add);

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) open_dialog()
{
  if (ImGui::BeginMenu("File"))
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

    ImGui::EndMenu();
  }
}