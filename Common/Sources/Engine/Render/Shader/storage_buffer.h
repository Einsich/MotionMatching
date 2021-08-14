#pragma once
#include <vector>
#include <string>

struct BufferField
{
  std::string name;
  int type, offset, size, stride, vectorOffset;
};
struct StorageBuffer
{
  std::string name;
  int binding;
  int size;
  std::vector<BufferField> fields;
};
