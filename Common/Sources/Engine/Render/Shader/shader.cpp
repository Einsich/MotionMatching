#include "shader.h"
#include <iostream>

static std::vector<std::pair<std::string, GLuint>> shaderList;
static Shader badShader("invalid shader", BAD_PROGRAM);


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
Shader get_shader(std::string shader_name, bool with_log)
{
  auto shader_iter = std::find_if(shaderList.begin(), shaderList.end(), [&](const auto &p){return p.first == shader_name;});
	if (shader_iter != shaderList.end())
  {
    return Shader(shader_iter->first, shader_iter->second);
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