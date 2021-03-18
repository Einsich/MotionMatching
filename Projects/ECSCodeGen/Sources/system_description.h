#pragma once
#include <vector>
#include <string>
#
struct FunctionArgument
{
    std::string type = "none", name = "none";
    bool optional = false;
};
struct SystemDesription
{
    std::string sys_file, sys_name;
    std::vector<FunctionArgument> args;
};