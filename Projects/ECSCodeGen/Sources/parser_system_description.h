#pragma once
#include <vector>
#include <string>
#include "string_hash.h"
struct ParserFunctionArgument
{
    std::string type, name;
    bool optional = false;
};
struct ParserSystemDescription
{
    std::string sys_file, sys_name;
    std::vector<ParserFunctionArgument> args;
};