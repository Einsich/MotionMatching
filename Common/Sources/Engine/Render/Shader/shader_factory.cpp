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

static map<fs::path, ShaderFileDependency> shaderFiles;

void load_file_dependencies(fs::path projectResources)
{
  projectResources.concat("/shaders_dependencies.bin");
  ifstream file(projectResources, ios::binary);
  shaderFiles.clear();
  read(file, shaderFiles);
}

void save_file_dependencies(fs::path projectResources)
{
  projectResources.concat("/shaders_dependencies.bin");
  ofstream file(projectResources, ios::binary);
  shaderFiles.clear();
  write(file, shaderFiles);
}
void update_file(const fs::path &file_path)
{
  fs::path path = file_path.lexically_relative(fs::path(Application::instance().projectPath));
  auto it = shaderFiles.find(path);
  if (it != shaderFiles.end())
  {
    ShaderFileDependency &dep = it->second;
    dep.exists = true;
    fs::file_time_type last_write = fs::last_write_time(file_path);
    dep.valid = last_write < dep.lastCompilationTime;
  }
  else
  {
    shaderFiles.try_emplace(path, ShaderFileDependency{{},{}, false, true});
  }
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
      add_shader_path(path);
    }
  }
}

void compile_shaders()
{
  fs::path projectResourcesPath = fs::path(Application::instance().projectResourcesPath);
  load_file_dependencies(projectResourcesPath);
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  process_codegen_shaders();
  debug_log("finish shader compilation");
}
void save_shader_info()
{
  fs::path projectResourcesPath = fs::path(Application::instance().projectResourcesPath);
  save_file_dependencies(projectResourcesPath);
}