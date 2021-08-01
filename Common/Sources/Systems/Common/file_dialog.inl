#include "ecs/ecs.h"
#include "Engine/input.h"
#include <windows.h>
#include "ecs/singleton.h"

struct FileDialog : ecs::Singleton
{
  OPENFILENAME ofn;
  char szFile[255];
  FileDialog()
  {
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.txt\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir=NULL;
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
  }
};

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) open_dialog(FileDialog &file_dialog)
{
  if (ImGui::BeginMenu("Scene"))
  {
    if (ImGui::Button("open file dialog"))
    {
      
      if (GetOpenFileName(&file_dialog.ofn))
      {
        debug_error("path: %s", file_dialog.ofn.lpstrFile);
      }
    }

    ImGui::EndMenu();
  }
}