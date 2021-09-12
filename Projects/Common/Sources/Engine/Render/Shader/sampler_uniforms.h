#pragma once
#include <string>
#include <vector>
struct SamplerUniform
{
  std::string name;
  unsigned int type;
  int shaderLocation, binding, arrayOffset;
};