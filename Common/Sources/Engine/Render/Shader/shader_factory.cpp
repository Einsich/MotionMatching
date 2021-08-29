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
static map<string, vector<pair<GLuint, string>>> shaderMap;

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
    if (entry.is_regular_file())
    {
      if (path.extension() == ".glsl")
      {
        add_shader_path(path);
        continue;
      }

      auto shaderTypeIter = shaderTypeMap.find(path.extension());
      if (shaderTypeIter != shaderTypeMap.end())
      {
        string shaderName = path.stem().string();
        ifstream shaderFile;
        shaderFile.exceptions(ifstream::badbit);
        try 
        {
          shaderFile.open(path);
          stringstream shaderStream;
          shaderStream << shaderFile.rdbuf();  
          shaderFile.close();

          shaderMap[shaderName].emplace_back(shaderTypeIter->second, shaderStream.str());
        }
        catch(ifstream::failure &e)
        {
          debug_error("Shader in %s not successfuly read!", path.string().c_str());
        }
      }
    }
    
  }
}

void compile_shaders(const map<string, vector<pair<GLuint, string>>> &shaderMap, bool createShaders)
{
  process_codegen_shaders();
  for (auto &shader:shaderMap)
  {
    GLuint program;
    if (compile_shader(shader.first, shader.second, program) && createShaders)
    {
      Shader(shader.first, program);
    }
  }
}
void precompile_shaders()
{
  shaderMap.clear();
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  compile_shaders(shaderMap, false);
}
void compile_shaders()
{
  shaderMap.clear();
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  compile_shaders(shaderMap, true);
  debug_log("finish shader compilation");
}