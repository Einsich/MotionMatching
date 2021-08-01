#include "ecs/ecs.h"
#include "Engine/input.h"
#include "Engine/file_dialog.h"

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) open_dialog()
{
  if (ImGui::BeginMenu("Scene"))
  {
    if (ImGui::Button("get open file"))
    {
      string fileName;
      if (get_open_file(fileName, {"Templates", ".tmpl"}))
      {
        debug_error("path: %s", fileName.c_str());
      }
    }
    if (ImGui::Button("get save file"))
    {
      string fileName;
      if (get_save_file(fileName, ".loh"))
      {
        debug_error("path: %s", fileName.c_str());
      }
    }

    ImGui::EndMenu();
  }
}