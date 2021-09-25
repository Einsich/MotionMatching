#pragma once
#include "shader_buffer.h"
#include "ecs/manager/is_vector.h"

template<typename T>
std::enable_if_t<!is_vector<T>::value, void> copy_buffer_field(const T &scr, char *data, const BufferField &field)
{
  memcpy(data + field.offset, &scr, sizeof(T));
}

template<typename T>
void copy_buffer_field(const std::vector<T> &scr, char *data, const BufferField &field)
{
  for (uint i = 0; i < scr.size(); i++)
    memcpy(data + field.offset + i * field.stride, &src[field.vectorOffset + i], sizeof(T));
}