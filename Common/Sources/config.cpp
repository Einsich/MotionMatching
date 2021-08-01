#include "config.h"
#include <map>
#include <filesystem>
static std::map<std::string, std::string> configs;

void add_configs(int config_count, const char** config_value)
{
  char key[255];
  char val[255];
  for (int i = 1; i < config_count; i++)
  {
    sscanf(config_value[i], "-%s -%s", key, val);
    configs[std::string(key)] = std::string(val);
  }
  std::string buildPath = std::filesystem::current_path().string();
  std::string project = configs["project"];
  configs["projectPath"] = buildPath + "\\..\\..\\..\\Projects\\" + project;
  configs["commonPath"] = buildPath + "\\..\\..\\..\\Common";
}
const char* get_config(const std::string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? nullptr : it->second.c_str();

}

bool get_bool_config(const std::string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? false : (it->second == "on" || it->second == "yes");
}