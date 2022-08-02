#pragma once
#include <vector>
#include <string>

struct BufferField
{
  std::string name;
  int type, offset, size, stride, vectorOffset;
};
struct SamplerUniform
{
  std::string name;
  unsigned int type;
  int shaderLocation, binding, arrayOffset;
};
struct ShaderBuffer
{
  int binding;
  int size;
  std::vector<BufferField> materialFields;
  vector<SamplerUniform> samplers;
};
