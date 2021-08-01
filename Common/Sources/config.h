#pragma once
#include <string>

void add_configs(int config_count, const char** config_value);
const char* get_config(const std::string &config_name);
bool get_bool_config(const std::string &config_name);

