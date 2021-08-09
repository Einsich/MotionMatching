#pragma once

void add_global_uniform(const char *name, size_t size, int binding);

template<typename T>
void add_global_uniform(const char *name, int binding)
{
  add_global_uniform(name, sizeof(T), binding);
}

void update_global_uniform(const char *name, const void *data, size_t size);
template<typename T>
void update_global_uniform(const char *name, const T &data)
{
  update_global_uniform(name, &data, sizeof(T));
}