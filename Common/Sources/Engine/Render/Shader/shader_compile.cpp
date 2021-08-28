#include "common.h"
#include "glad/glad.h"
#include <filesystem>
#include <vector>
#include <map>

namespace fs = filesystem;

fs::path shader_extension(GLuint shader);
bool compile_shader(const string &shaderName, const vector<pair<GLuint, const char*>> &shaders, GLuint &program)
{
    vector<GLuint> compiled_shaders;
    compiled_shaders.reserve(shaders.size());
    GLchar infoLog[512];
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

        debug_error("Shader (%s.%s) compilation failed!\n Log: %s", shaderName.c_str(), extension.string().c_str(), infoLog);
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
        glGetProgramInfoLog(program, 512, NULL, infoLog);
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