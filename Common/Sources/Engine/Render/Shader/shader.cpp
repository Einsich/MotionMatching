#include "shader.h"
#include <iostream>

static std::vector<std::pair<std::string, GLuint>> shaderList;
static Shader badShader("badShader", BAD_PROGRAM);


Shader::Shader(const std::string &shader_name, GLuint shader_program, bool update_list)
{
  for (shaderIdx = 0; shaderIdx < (int)shaderList.size() && shaderList[shaderIdx].first != shader_name; ++shaderIdx);

  if (shaderIdx >= (int)shaderList.size())
  {
    shaderList.emplace_back(shader_name, shader_program);
  }
  if (update_list)
  {
    shaderList[shaderIdx] = make_pair(shader_name, shader_program);
  }
}
GLuint Shader::get_shader_program() const
{
  return shaderList[shaderIdx].second;
}
Shader get_shader(std::string shader_name)
{
  auto shader_iter = std::find_if(shaderList.begin(), shaderList.end(), [&](const auto &p){return p.first == shader_name;});
	if (shader_iter != shaderList.end())
  {
    return Shader(shader_iter->first, shader_iter->second);
  }
  else
  {
    std::cerr << "Can't find shader " << shader_name << std::endl;
    return badShader;
  }
}
void Shader::use() const
{
  glUseProgram(get_shader_program());
}