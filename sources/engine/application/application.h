#pragma once
#include <string>
#include <filesystem>

std::string project_path(const std::string &path);
std::string project_path();
std::string root_path(const std::string &path);
std::string root_path();

//width, heigth
std::pair<int,int> get_resolution();
void create_scene(const char *path);