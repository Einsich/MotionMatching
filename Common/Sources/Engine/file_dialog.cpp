#include "file_dialog.h"
#include <windows.h>

char *add_str(char *buf, const std::string &str)
{
  for (unsigned int i = 0; i < str.size(); ++i, ++buf)
    *buf = str[i];
  *buf = '\0';
  return buf;
}
bool get_filename_impl(BOOL (*open_file)(OPENFILENAME*), char *file, int file_size, uint32_t flags, 
  const std::vector<std::pair<std::string, std::string>> &extensions)
{
  constexpr int N = 255;
  char filter[N];
  char *bufPtr = filter;
  for (const auto &p : extensions)
  {
    bufPtr = add_str(bufPtr, p.first) + 1;
    bufPtr = add_str(bufPtr, "*"+p.second) + 1;
  }

  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = file;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = file_size;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir=NULL;
  ofn.Flags = flags;
  return open_file(&ofn);
}
bool get_open_file_impl(char *file, int file_size, const std::pair<std::string, std::string> &extension)
{
  return get_filename_impl(GetOpenFileName, file, file_size, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, {extension});
}
bool get_open_file(std::string &path, const std::pair<std::string, std::string> &extension)
{
  constexpr int N = 255;
  char file[N];
  bool success = get_open_file_impl(file, N, extension);
  if (success)
    path.assign(file);
  return success;
}

bool get_open_file(std::filesystem::path &path, const std::pair<std::string, std::string> &extension)
{
  constexpr int N = 255;
  char file[N];
  bool success = get_open_file_impl(file, N, extension);
  if (success)
    path.assign(file);
  return success;
}
bool get_open_file(char *path, int max_path, const std::pair<std::string, std::string> &extension)
{
  return get_open_file_impl(path, max_path, extension);
}
bool get_save_file_impl(char *file, int file_size, const std::string &extension)
{
  bool success = get_filename_impl(GetSaveFileName, file, file_size, OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, {{extension, extension}});
  if (success)
    sprintf(file + strlen(file), extension.c_str());
  return success;
}
bool get_save_file(std::string &path, const std::string &extension)
{
  constexpr int N = 255;
  char file[N];
  bool success = get_save_file_impl(file, N, extension);
  if (success)
    path.assign(file);
  return success;
}

bool get_save_file(std::filesystem::path &path, const std::string &extension)
{
  constexpr int N = 255;
  char file[N];
  bool success = get_save_file_impl(file, N, extension);
  if (success)
    path.assign(file);
  return success;
}
bool get_save_file(char *path, int max_path, const std::string &extension)
{
  return get_save_file_impl(path, max_path, extension);
}