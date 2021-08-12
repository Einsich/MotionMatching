#pragma once
#include <vector>
struct UniformBuffer
{
private:
  uint arrayID;
  uint BUF_TYPE;
  int bindID;
  std::vector<char> buffer;
  std::string name;
public:
  UniformBuffer(uint arrayID, uint BUF_TYPE, int bindID, std::vector<char> &&buffer, std::string && name):
    arrayID(arrayID), BUF_TYPE(BUF_TYPE), bindID(bindID), buffer(buffer), name(name){}
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
  void flush_buffer() const;
  char *get_buffer();
};
void add_uniform_buffer(const char *name, size_t size, int binding);
void add_storage_buffer(const char *name, size_t size, int binding);
template<typename T>
void add_uniform_buffer(const char *name, int binding)
{
  add_uniform_buffer(name, sizeof(T), binding);
}
UniformBuffer &get_buffer(const char *name);