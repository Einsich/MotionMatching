#pragma once
#include <string>
#include <filesystem>

std::string project_path(const std::string &path);
const std::string &project_path();

inline const char *project_path_str()
{
  return project_path().c_str();
}
std::string root_path(const std::string &path);
const std::string &root_path();

inline const char *root_path_ctr()
{
  return root_path().c_str();
}

//width, heigth
std::pair<int,int> get_resolution();
void create_scene(const char *path);