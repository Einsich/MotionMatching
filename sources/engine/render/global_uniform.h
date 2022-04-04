#pragma once
#include <vector>
struct UniformBuffer
{
private:
  uint arrayID;
  uint bufType;
  int bindID;
  mutable uint bufSize;
  mutable std::vector<char> buffer;
  std::string name;
  void resize(uint new_size) const;
public:
  UniformBuffer(uint arrayID, uint bufType, int bindID, uint bufSize, std::vector<char> &&buffer, std::string && name):
    arrayID(arrayID), bufType(bufType), bindID(bindID), bufSize(bufSize), buffer(buffer), name(name){}
  size_t size() const;
  //don't use temporary buffer
  void update_buffer_and_flush(const void *data, size_t size) const;
  //don't use temporary buffer
  template<typename T>
  void update_buffer_and_flush(const T &data) const
  {
    update_buffer_and_flush(&data, sizeof(T));
  }
  //uses temporary buffer
  void update_buffer(const void *data, size_t offset, size_t size) const;
  void flush_buffer(size_t flush_size) const;
  char *get_buffer(uint offset, uint size);
  void bind() const;
};
void add_uniform_buffer(const char *name, size_t size, int binding);
void add_storage_buffer(const char *name, size_t size, int binding);
template<typename T>
void add_uniform_buffer(const char *name, int binding)
{
  add_uniform_buffer(name, sizeof(T), binding);
}
UniformBuffer &get_buffer(const char *name);