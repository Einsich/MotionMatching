#include <map>
#include "common.h"
#include "global_uniform.h"
#include "glad/glad.h"

static map<string, UniformBuffer> nameToArray;

template<uint BUF_TYPE>
void add_buffer(const char *name, size_t size, int binding)
{
  auto it = nameToArray.find(name);
  if (it == nameToArray.end())
  {
    uint uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(BUF_TYPE, uniformBuffer);
    glBufferData(BUF_TYPE, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(BUF_TYPE, 0);
    nameToArray.try_emplace(name, uniformBuffer, BUF_TYPE, binding, size, vector<char>(size), string(name));
  }
}
void add_uniform_buffer(const char *name, size_t size, int binding)
{
  add_buffer<GL_UNIFORM_BUFFER>(name, size, binding);
}
void add_storage_buffer(const char *name, size_t size, int binding)
{
  add_buffer<GL_SHADER_STORAGE_BUFFER>(name, size, binding);
}
size_t UniformBuffer::size() const
{
  return buffer.size();
}
void UniformBuffer::update_buffer_and_flush(const void *data, size_t size) const
{
  glBindBuffer(bufType, arrayID);
  glBindBufferBase(bufType, bindID, arrayID); 
  if (bufSize < size)
  {
    glBufferData(bufType, size, NULL, GL_DYNAMIC_DRAW);
    bufSize = size;
  }
  glBufferSubData(bufType, 0, size, data); 
  glBindBuffer(bufType, 0);
}

//uses temporary buffer
void UniformBuffer::update_buffer(const void *data, size_t offset, size_t size) const
{
  if (this->size() < size)
    buffer.resize(size);
  memcpy((char*)buffer.data() + offset, data, size);
}
void UniformBuffer::flush_buffer(size_t flush_size) const
{
  if (bufSize < size()) 
  {
    glBufferData(bufType, size(), NULL, GL_DYNAMIC_DRAW);
    bufSize = size();
  }
  glBufferSubData(bufType, 0, min(size(), flush_size), buffer.data());
}
char *UniformBuffer::get_buffer(uint offset, uint size)
{
  if (buffer.size() < offset + size)
    buffer.resize(offset + size);

  return buffer.data() + offset;
}

void UniformBuffer::bind() const
{
  glBindBuffer(bufType, arrayID);
  glBindBufferBase(bufType, bindID, arrayID); 
}

UniformBuffer &get_buffer(const char *name)
{
  static UniformBuffer invalidBuffer(0, 0, 0, 0, {}, "");
  auto it = nameToArray.find(name);
  return it != nameToArray.end() ? it->second : invalidBuffer;
}