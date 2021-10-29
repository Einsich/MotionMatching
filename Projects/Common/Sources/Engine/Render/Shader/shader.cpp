#include "shader.h"
#include <iostream>
#include <map>
#include "shader_gen.h"

struct ShaderInfo
{
  GLuint program;
  bool compiled;//or loaded
  ShaderBuffer buffer; 
};
void read_shader_info(const std::string &, ShaderInfo &shader);

static std::vector<std::pair<std::string, ShaderInfo>> shaderList;
static Shader badShader(-1);


Shader::Shader(const std::string &shader_name, GLuint shader_program, bool compiled, bool update_list)
{
  if (shader_program == BAD_PROGRAM)
    return;
  for (shaderIdx = 0; shaderIdx < (int)shaderList.size() && shaderList[shaderIdx].first != shader_name; ++shaderIdx);

  if (shaderIdx >= (int)shaderList.size())
  {
    shaderList.emplace_back(shader_name, ShaderInfo{shader_program, compiled, {}});
    read_shader_info(shaderList.back().first, shaderList.back().second);
  }
  else
  if (update_list)
  {
    glDeleteProgram(shaderList[shaderIdx].second.program);
    shaderList[shaderIdx] = make_pair(shader_name, ShaderInfo{shader_program, compiled, {}});
    read_shader_info(shaderList[shaderIdx].first, shaderList[shaderIdx].second);
  }
}
GLuint Shader::get_shader_program() const
{
  return shaderIdx < 0 ? BAD_PROGRAM : shaderList[shaderIdx].second.program;
}

const string& Shader::get_name() const
{
  static string dummy = "";
  return shaderIdx < 0 ? dummy : shaderList[shaderIdx].first;
}

int get_shader_index(const std::string &shader_name)
{
  auto shader_iter = std::find_if(shaderList.begin(), shaderList.end(), [&](const auto &p){return p.first == shader_name;});
  return shader_iter != shaderList.end() ? shader_iter - shaderList.begin() : -1;
}
Shader get_shader(const std::string &shader_name, bool with_log)
{
  int shaderIdx = get_shader_index(shader_name);
	if (shaderIdx >= 0)
  {
    return Shader(shaderIdx);
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

void read_shader_info(const std::string &shader_name, ShaderInfo &shader)
{
  GLuint program = shader.program;
  if (!program)
    return;
  ShaderBuffer &buffer = shader.buffer;
  buffer.materialFields.clear();
  vector<SamplerUniform> &samplers = buffer.samplers;
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
    //debug_log("uniform #%d Type: %u Name: %s", i, type, name);

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
      //debug_log("Sampler #%d Type: %u Name: %s, Location %d", i, type, name, texLocation);
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

    //debug_log("buffer %s (binding = %d), size %d", name, binding, size);

    property = GL_ACTIVE_VARIABLES;
    vector<GLint> varId(numVar);
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, resInx, 1, &property, varId.size(), nullptr, varId.data());
    bool isInstance = !strcmp(name, "InstanceData");
    int bufferNameLen = isInstance ? sizeof("instances[0]") : 0;
    if (isInstance)
    {
      buffer.binding = binding;
      buffer.size = size;
      buffer.Model = buffer.Bones = BufferField();
    }
    else
    {
      debug_error("Shader %s contains %s uniform buffer. Allow only instance uniform buffer", shader_name.c_str(), name);
      continue;
    }
    vector<BufferField> &fields = buffer.materialFields;
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
      char *materialPart = strstr(matterPart, "material.");
      if (materialPart)
      {
        fields.emplace_back(BufferField{string(materialPart), params[0], params[1], params[2], params[3], typeToOffset[params[0]]});
        typeToOffset[params[0]] += params[2];
      }
      else
      {
        if (!strcmp(matterPart, "Model"))
          buffer.Model = BufferField{string(matterPart), params[0], params[1], params[2], params[3], 0};
        else if (!strcmp(matterPart, "Bones[0]"))
          buffer.Bones = BufferField{string(matterPart), params[0], params[1], params[2], params[3], 0};
        else
        {
          fields.emplace_back(BufferField{string(matterPart), params[0], params[1], params[2], params[3], typeToOffset[params[0]]});
          typeToOffset[params[0]] += params[2];
        }
      }
      //debug_log("Field[%d] %s, type %d offset %d, size %d, array stride %d", i, matterPart, params[0], params[1], params[2], params[3]);
    }
  }
}
const ShaderBuffer &Shader::get_instance_data() const
{
  return shaderList[shaderIdx].second.buffer;
}

const vector<SamplerUniform> &Shader::get_samplers() const
{
  return shaderList[shaderIdx].second.buffer.samplers;
}

void load_precompiled_shaders(const vector<PrecompiledShader> &shaders)
{
  for (const PrecompiledShader &shader : shaders)
  {
    GLuint program = 0;
    if (load_precompiled_shader(shader, program))
    {
      debug_log("loaded precompiled %s", shader.name.c_str());
      shaderList.emplace_back(shader.name, ShaderInfo{program, false, {}});
      read_shader_info(shaderList.back().first, shaderList.back().second);
    }
  }
}

void save_precompiled_shaders(vector<PrecompiledShader> &shaders)
{
  for (const auto &info : shaderList)
  {
    if (info.second.compiled)//not loaded
    {
      GLuint program = info.second.program;
      debug_log("saved precompiled %s", info.first.c_str());
      auto shader_iter = std::find_if(shaders.begin(), shaders.end(), [&](const auto &p){return p.name == info.first;});
      if (shader_iter != shaders.end())
      {
        save_precompiled_shader(*shader_iter, program);
      }
      else
      {
        PrecompiledShader &shader = shaders.emplace_back(PrecompiledShader{info.first, {}, 0});
        save_precompiled_shader(shader, program);
      }
    }
  }
}