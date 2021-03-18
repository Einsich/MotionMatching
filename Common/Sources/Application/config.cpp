#include "config.h"
#include <map>
static map<string, string> configs;

void add_configs(int config_count, const char** config_value)
{
  char key[255];
  char val[255];
  for (int i = 0; i < config_count; i++)
  {
    sscanf(config_value[i], "-%s -%s", key, val);
    configs[string(key)] = string(val);
  }
}
const char* get_config(const string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? nullptr : it->second.c_str();

}

bool get_bool_config(const string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? false : (it->second == "on" || it->second == "yes");
}