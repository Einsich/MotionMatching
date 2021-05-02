#include "shader.h"
#include <iostream>

static std::map<std::string, GLuint> shaderMap;
static Shader badShader("badShader", BAD_PROGRAM);
Shader::Shader(map<string, GLuint>::iterator program):program(program){}

Shader::Shader(std::string shader_name, unsigned int shader_program)
{
  program = shaderMap.find(shader_name);
	if (program != shaderMap.end())
  {
    //glad_glDeleteShader(shader_iter->second.program);
    program->second = shader_program;
  }
  else
  {
    shaderMap[shader_name] = shader_program;
    program = shaderMap.find(shader_name);
  }
}
Shader get_shader(std::string shader_name)
{
  auto shader_iter = shaderMap.find(shader_name);
	if (shader_iter != shaderMap.end())
  {
    return Shader(shader_iter);
  }
  else
  {
    std::cerr << "Can't find shader " << shader_name << std::endl;
    return badShader;
  }
}
void Shader::use() const
{
  glUseProgram(program->second);
}