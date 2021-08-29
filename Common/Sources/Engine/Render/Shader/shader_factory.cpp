#include "common.h"
#include "shader_factory.h"
#include "shader.h"
#include "Application/application.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>

namespace fs = filesystem;

void add_shader_path(const fs::path &path);
bool compile_shader(const string &shaderName, const vector<pair<GLuint, string>> &shaders, GLuint &program);
void process_codegen_shaders();


void read_directories(string path)
{
  if (!fs::exists(path))
  {
    debug_error("Didn't exists %s", path.c_str());
    return;
  }
  for (const auto & entry : fs::directory_iterator(path))
  {
    const fs::path &path = entry.path();
    
    if (entry.is_directory())
    {
      read_directories(entry.path().string());
    } 
    else
    if (entry.is_regular_file() && path.extension() == ".glsl")
    {
      add_shader_path(path);
    }
    
  }
}

void precompile_shaders()
{
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  process_codegen_shaders();
}
void compile_shaders()
{
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  process_codegen_shaders();
  debug_log("finish shader compilation");
}