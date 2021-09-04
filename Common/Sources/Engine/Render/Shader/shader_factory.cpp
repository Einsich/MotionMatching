#include "common.h"
#include "shader_factory.h"
#include "shader.h"
#include "shader_gen.h"
#include "Application/application.h"
#include "Serialization/serialization.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>

namespace fs = filesystem;

static vector<PrecompiledShader> precompiledShaders;

template<typename T>
void load_obj(const char *file_name, T & obj)
{
  ifstream file(fs::path(Application::instance().projectResourcesPath).concat(file_name), ios::binary);
  obj.clear();
  read(file, obj);
}

template<typename T>
void save_obj(const char *file_name, T & obj)
{
  ofstream file(fs::path(Application::instance().projectResourcesPath).concat(file_name), ios::binary);
  write(file, obj);
}


void add_shader_path(const fs::path &path);
bool compile_shader(const string &shaderName, const vector<pair<GLuint, string>> &shaders, GLuint &program);
void process_codegen_shaders();


void read_directories(const fs::path &folder_path)
{
  if (!fs::exists(folder_path))
  {
    debug_error("Didn't exists %s", folder_path.string().c_str());
    return;
  }
  for (const auto & entry : fs::directory_iterator(folder_path))
  {
    const fs::path &path = entry.path();
    if (entry.is_directory())
    {
      read_directories(entry.path());
    } 
    else if (entry.is_regular_file() && path.extension() == ".glsl")
    {
      update_file(path);
    }
  }
}

void compile_shaders()
{
  load_obj("/shaders_dependencies.bin", getShaderFiles());
  load_obj("/precompiled_shaders.bin", precompiledShaders);
  load_precompiled_shaders(precompiledShaders);
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  process_codegen_shaders();
  debug_log("finish shader compilation");
}
void recompile_shaders()
{
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  process_codegen_shaders();
  debug_log("finish shader recompilation");
}
void save_shader_info()
{
  save_precompiled_shaders(precompiledShaders);
  save_obj("/shaders_dependencies.bin", getShaderFiles());
  save_obj("/precompiled_shaders.bin", precompiledShaders);
}