#include <map>
#include "common.h"
#include "glad/glad.h"
struct UniformBuffer
{
  uint arrayID;
  int bindID;
};
static map<string, UniformBuffer> nameToArray;

void add_global_uniform(const char *name, size_t size, int binding)
{
  auto it = nameToArray.find(name);
  if (it == nameToArray.end())
  {
    uint uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    nameToArray.emplace(name, UniformBuffer{uniformBuffer, binding});
  }
}

void update_global_uniform(const char *name, const void *data, size_t size)
{
  auto it = nameToArray.find(name);
  if (it != nameToArray.end())
  {
    UniformBuffer ub = it->second;
    glBindBuffer(GL_UNIFORM_BUFFER, ub.arrayID);
    glBindBufferBase(GL_UNIFORM_BUFFER, ub.bindID, ub.arrayID); 
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  else
    debug_error("Can't find %s uniform buffer", name);
}