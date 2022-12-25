#pragma once
#include <eastl/vector.h>
#include "shader_buffer.h"
#include "utils/is_vector.h"

template<typename T>
std::enable_if_t<!is_vector<T>::value, void> copy_buffer_field(const T &scr, char *data, const BufferField &field)
{
  memcpy(data + field.offset, &scr, sizeof(T));
}

template<typename T>
void copy_buffer_field(const std::vector<T> &src, char *data, const BufferField &field)
{
  for (uint i = 0; i < src.size(); i++)
    memcpy(data + field.offset + i * field.stride, &src[field.vectorOffset + i], sizeof(T));
}
template<typename T>
void copy_buffer_field(const eastl::vector<T> &src, char *data, const BufferField &field)
{
  for (uint i = 0; i < src.size(); i++)
    memcpy(data + field.offset + i * field.stride, &src[field.vectorOffset + i], sizeof(T));
}