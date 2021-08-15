#include "shader.h"
#include <iostream>
#include <map>
#include "storage_buffer.h"
#include "sampler_uniforms.h"

struct ShaderInfo
{
  GLuint program;
  int instanceDataBuffer;
  vector<StorageBuffer> buffers; 
  vector<SamplerUniform> samplers;
};
void read_shader_info(ShaderInfo &shader);

static std::vector<std::pair<std::string, ShaderInfo>> shaderList;
static Shader badShader("invalid shader", BAD_PROGRAM);


Shader::Shader(const std::string &shader_name, GLuint shader_program, bool update_list)
{
  for (shaderIdx = 0; shaderIdx < (int)shaderList.size() && shaderList[shaderIdx].first != shader_name; ++shaderIdx);

  if (shaderIdx >= (int)shaderList.size())
  {
    shaderList.emplace_back(shader_name, ShaderInfo{shader_program, -1, {}, {}});
    read_shader_info(shaderList.back().second);
  }
  if (update_list)
  {
    shaderList[shaderIdx] = make_pair(shader_name, ShaderInfo{shader_program, -1, {}, {}});
    read_shader_info(shaderList[shaderIdx].second);
  }
}
GLuint Shader::get_shader_program() const
{
  return shaderList[shaderIdx].second.program;
}
Shader get_shader(std::string shader_name, bool with_log)
{
  auto shader_iter = std::find_if(shaderList.begin(), shaderList.end(), [&](const auto &p){return p.first == shader_name;});
	if (shader_iter != shaderList.end())
  {
    return Shader(shader_iter->first, shader_iter->second.program);
  }
  else
  {
    if (with_log)
      debug_error("Can't find shader %s", shader_name.c_str());
    return badShader;
  }
}
void Shader::use() const
{
  glUseProgram(get_shader_program());
}
const vector<const char*>get_shaders_names()
{
  std::vector<const char *> shadersNames(shaderList.size());
  for (uint i = 0; i < shaderList.size(); ++i)
    shadersNames[i] = shaderList[i].first.c_str();
  return shadersNames;
}

void read_shader_info(ShaderInfo &shader)
{
  GLuint program = shader.program;
  if (!program)
    return;
  shader.instanceDataBuffer = -1;
  vector<StorageBuffer> &buffers = shader.buffers;
  buffers.clear();
  vector<SamplerUniform> &samplers = shader.samplers;
  samplers.clear();
  int count;
  const GLsizei bufSize = 128;
  GLchar name[bufSize];
  GLsizei length;
  GLenum property;

  map<uint, int> typeToOffset;
  glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
  for (int i = 0, texBinding = 0; i < count; i++)
  {
    GLenum type;
    GLint size;
    glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
    switch (type)
    {
    //case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    //case GL_SAMPLER_1D_SHADOW:
    //case GL_SAMPLER_2D_SHADOW:

      GLint texLocation = glGetUniformLocation(program, name);
      samplers.emplace_back(SamplerUniform{string(name), type, texLocation, texBinding, typeToOffset[type]});
      debug_log("Sampler #%d Type: %u Name: %s, Location %d", i, type, name, texLocation);
      texBinding++;
      typeToOffset[type]++;
      break;
    }
  }

  typeToOffset.clear();
  glGetProgramInterfaceiv(program, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &count);
  for (int i = 0; i < count; i++)
  {
    glGetProgramResourceName(program, GL_SHADER_STORAGE_BLOCK, i, bufSize, &length, name);
    GLint resInx = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, name);

    property = GL_NUM_ACTIVE_VARIABLES;
    GLint numVar;
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, resInx, 1, &property, 1, nullptr, &numVar);
    property = GL_BUFFER_BINDING;
    GLint binding;
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, resInx, 1, &property, 1, nullptr, &binding);
    property = GL_BUFFER_DATA_SIZE;
    GLint size;
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, resInx, 1, &property, 1, nullptr, &size);

    debug_log("buffer %s (binding = %d), size %d", name, binding, size);
    buffers.emplace_back(StorageBuffer{string(name), binding, size, vector<BufferField>(numVar)});
    vector<BufferField> &fields = buffers.back().fields;
    property = GL_ACTIVE_VARIABLES;
    vector<GLint> varId(numVar);
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, resInx, 1, &property, varId.size(), nullptr, varId.data());
    bool isInstance = !strcmp(name, "InstanceData");
    int bufferNameLen = isInstance ? sizeof("instances[0]") : 0;
    if (isInstance)
      shader.instanceDataBuffer = i;
    for (GLint i = 0; i < numVar; i++) 
    {
      constexpr int N = 4;
      GLenum properties[N] = {GL_TYPE, GL_OFFSET, GL_ARRAY_SIZE, GL_ARRAY_STRIDE};
      GLint params[N];
      glGetProgramResourceiv(
          program, GL_BUFFER_VARIABLE, varId[i],
          N, properties, N, nullptr, params);
        GLsizei strLength;
        glGetProgramResourceName(
            program, GL_BUFFER_VARIABLE, varId[i], 
            bufSize, &strLength, name);
      char *matterPart = name + bufferNameLen;
      fields[i] = BufferField{string(matterPart), params[0], params[1], params[2], params[3], typeToOffset[params[0]]};
      typeToOffset[params[0]] += params[2];
      debug_log("Field %s, type %d offset %d, size %d, array stride %d", matterPart, params[0], params[1], params[2], params[3]);
    }
  }
}
const StorageBuffer *Shader::get_instance_data() const
{
  int index = shaderList[shaderIdx].second.instanceDataBuffer;
  return index >=0 ? &shaderList[shaderIdx].second.buffers[index] : nullptr;
}

const vector<SamplerUniform> &Shader::get_samplers() const
{
  return shaderList[shaderIdx].second.samplers;
}