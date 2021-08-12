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
    nameToArray.try_emplace(name, uniformBuffer, BUF_TYPE, binding, vector<char>(size), string(name));
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
  if (this->size() < size)
    debug_error("try pass in %s buffer %d byte, it can storage %d byte", name.c_str(), size, this->size());
  glBindBuffer(BUF_TYPE, arrayID);
  glBindBufferBase(BUF_TYPE, bindID, arrayID); 
  glBufferSubData(BUF_TYPE, 0, size, data); 
  glBindBuffer(BUF_TYPE, 0);
}

//uses temporary buffer
void UniformBuffer::update_buffer(const void *data, size_t offset, size_t size) const
{
  if (this->size() < offset + size)
    debug_error("try pass in %s buffer %d byte with offset %d (end in %d), it can storage %d byte", name.c_str(), size, offset, size + offset, this->size());
  memcpy((char*)buffer.data() + offset, data, size);
}
void UniformBuffer::flush_buffer() const
{
  glBindBuffer(BUF_TYPE, arrayID);
  glBindBufferBase(BUF_TYPE, bindID, arrayID); 
  glBufferSubData(BUF_TYPE, 0, size(), buffer.data()); 
  glBindBuffer(BUF_TYPE, 0);
}
char *UniformBuffer::get_buffer()
{
  return buffer.data();
}

UniformBuffer &get_buffer(const char *name)
{
  static UniformBuffer invalidBuffer(0, 0, 0, {}, "");
  auto it = nameToArray.find(name);
  return it != nameToArray.end() ? it->second : invalidBuffer;
}