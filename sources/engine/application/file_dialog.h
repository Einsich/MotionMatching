#pragma once
#include <string>
#include <filesystem>
namespace filesystem = std::filesystem;
bool get_open_file(std::string &path, const std::pair<std::string, std::string> &extension = {"All", "*.*"});
bool get_open_file(std::filesystem::path &path, const std::pair<std::string, std::string> &extension = {"All", "*.*"});
bool get_open_file(char *path, int max_path, const std::pair<std::string, std::string> &extension = {"All", "*.*"});

bool get_save_file(std::string &path, const std::string &extension);
bool get_save_file(std::filesystem::path &path, const std::string &extension);
bool get_save_file(char *path, int max_path, const std::string &extension);