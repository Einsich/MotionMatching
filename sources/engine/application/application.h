#pragma once
#include <string>
#include<filesystem>

void load_scene(const std::string &name, bool editor);

std::string project_path(const std::string &path);
std::string project_path();
std::string root_path(const std::string &path);
std::string root_path();

void add_open_scene(const std::filesystem::path &path, bool need_to_add = true, bool need_to_open = false);