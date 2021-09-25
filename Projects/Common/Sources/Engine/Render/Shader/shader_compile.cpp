#include "common.h"
#include "glad/glad.h"
#include <filesystem>
#include <vector>
#include <map>
#include "shader_gen.h"

namespace fs = filesystem;

static map<fs::path, GLuint> shaderTypeMap = 
{{".vert", GL_VERTEX_SHADER}, {".frag", GL_FRAGMENT_SHADER}, {".comp", GL_COMPUTE_SHADER}, 
{".geom", 0}, {".tesc", 0}, {".tese", 0}};

fs::path shader_extension(GLuint shader)
{
  fs::path extension;
  for (auto p: shaderTypeMap)
    if (p.second == shader)
      extension = p.first;
  return extension;
}

bool compile_shader(const string &shaderName, const vector<pair<GLuint, const char*>> &shaders, GLuint &program)
{
    vector<GLuint> compiled_shaders;
    compiled_shaders.reserve(shaders.size());
    GLchar infoLog[1024];
    GLint success;
    for (auto & codes : shaders)
    {
      GLuint shaderProg = glCreateShader(codes.first);
      const GLchar * shaderCode = codes.second;
      glShaderSource(shaderProg, 1, &shaderCode, NULL);
      glCompileShader(shaderProg);
      glGetShaderiv(shaderProg, GL_COMPILE_STATUS, &success);
      if(!success)
      {
        glGetShaderInfoLog(shaderProg, 512, NULL, infoLog);
        fs::path extension = shader_extension(codes.first);

        debug_error("Shader (%s%s) compilation failed!\n Log: %s", shaderName.c_str(), extension.string().c_str(), infoLog);
        return false;
      };
      compiled_shaders.push_back(shaderProg);
    }


    program = glCreateProgram();
    for (GLuint shaderProg: compiled_shaders)
      glAttachShader(program, shaderProg);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        debug_error("Shader programm (%s) linking failed!\n Log: %s", shaderName.c_str(), infoLog);
        return false;
    }

    for (GLuint shaderProg: compiled_shaders)
      glDeleteShader(shaderProg);
  return true;
}

bool compile_shader(const string &shaderName, const vector<pair<GLuint, string>> &shaders, GLuint &program)
{
  vector<pair<GLuint, const char *>> shadersCopy;
  for (const auto &p :shaders)
    shadersCopy.emplace_back(p.first, p.second.c_str());
  return compile_shader(shaderName, shadersCopy, program);
}
void save_precompiled_shader(PrecompiledShader &shader, const GLuint &program)
{
  GLint length = 0;
  glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length);

  shader.data.resize(length);
  glGetProgramBinary(program, length, &length, &shader.format, shader.data.data());
GLuint p =0;
  load_precompiled_shader(shader, p);
}
bool load_precompiled_shader(const PrecompiledShader &shader, GLuint &program)
{
  program = glCreateProgram();
  glProgramBinary(program, shader.format, shader.data.data(), shader.data.size());

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status)
  {
    GLchar infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    debug_error("Shader programm (%s) linking failed!\n Log: %s", shader.name.c_str(), infoLog);
    return false;
  }
  return true;
}