#include "common.h"
#include "shader_factory.h"
#include "shader.h"
#include "shader_gen.h"
#include "application/application_data.h"
#include "serialization/serialization.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>

namespace fs = filesystem;

static std::vector<PrecompiledShader> precompiledShaders;

template<typename T>
void load_obj(const char *file_name, T & obj)
{
  ifstream file(Application::instance().projectPath + "/" + file_name, ios::binary);
  obj.clear();
  read(file, obj);
}

template<typename T>
void save_obj(const char *file_name, T & obj)
{
  ofstream file(Application::instance().projectPath + "/" + file_name, ios::binary);
  write(file, obj);
}


void add_shader_path(const fs::path &path);
bool compile_shader(const std::string &shaderName, const std::vector<std::pair<GLuint, std::string>> &shaders, GLuint &program);
void process_codegen_shaders();


void read_directories(const fs::path &folder_path)
{
  if (!fs::exists(folder_path))
  {
    debug_error("Didn't exists %s", folder_path.string().c_str());
    return;
  }
  for (const auto &entry : fs::recursive_directory_iterator(folder_path, fs::directory_options::follow_directory_symlink))
  {
    if (entry.is_regular_file() && entry.path().extension() == ".glsl")
    {
      update_file(entry.path());
    }
  }
}

void compile_shaders()
{
  //load_obj("/precompiled_shaders.bin", precompiledShaders);
  //load_precompiled_shaders(precompiledShaders);
  
  //load_obj("/shaders_dependencies.bin", getShaderFiles());
  for (const auto &path: Application::instance().shadersPaths)
    if (fs::exists(path))
      read_directories(path);
  process_codegen_shaders();

  debug_log("finish shader compilation");
}
void recompile_shaders()
{
#ifndef RELEASE
  for (const auto &path: Application::instance().shadersPaths)
    if (fs::exists(path))
      read_directories(path);
  process_codegen_shaders();
  debug_log("finish shader recompilation");
#endif
}
void save_shader_info()
{
#ifndef RELEASE
  //save_precompiled_shaders(precompiledShaders);
  //save_obj("/shaders_dependencies.bin", getShaderFiles());
  //save_obj("/precompiled_shaders.bin", precompiledShaders);
#endif
}