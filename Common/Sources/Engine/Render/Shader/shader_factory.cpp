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
enum SHADER_TYPE
{
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
  COMPUTE_SHADER = GL_COMPUTE_SHADER,
  GEOMETRY_SHADER,
  TESSELATION_CONTROL_SHADER,
  TESSELATION_EVALUATION_SHADER
};
map<fs::path, SHADER_TYPE> shaderTypeMap = {{".vert", VERTEX_SHADER}, {".frag", FRAGMENT_SHADER}, {".comp", COMPUTE_SHADER}, 
{".geom", GEOMETRY_SHADER}, {".tesc", TESSELATION_CONTROL_SHADER}, {".tese", TESSELATION_EVALUATION_SHADER}};

map<string, vector<pair<SHADER_TYPE, string>>> shaderMap;
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
      
      if (path.extension() == ".shader")
      {

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

void compile_shaders(const map<string, vector<pair<SHADER_TYPE, string>>> &shaderMap, bool createShaders)
{

  for (auto &shader:shaderMap)
  {
    string shaderName = shader.first;
    vector<GLuint> shaders;
    GLchar infoLog[512];
    GLint success;
    for (auto & codes : shader.second)
    {
      GLuint shaderProg = glCreateShader(codes.first);
      const GLchar * shaderCode = codes.second.c_str();
      glShaderSource(shaderProg, 1, &shaderCode, NULL);
      glCompileShader(shaderProg);
      glGetShaderiv(shaderProg, GL_COMPILE_STATUS, &success);
      if(!success)
      {
          glGetShaderInfoLog(shaderProg, 512, NULL, infoLog);
          fs::path extension;
          for (auto p: shaderTypeMap)
            if (p.second == codes.first)
              extension = p.first;

        debug_error("Shader (%s.%s) compilation failed!\n Log: %s", shaderName.c_str(), extension.string().c_str(), infoLog);
      };
      shaders.push_back(shaderProg);
    }


    GLuint program = glCreateProgram();
    for (GLuint shaderProg: shaders)
      glAttachShader(program, shaderProg);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        debug_error("Shader programm (%s) linking failed!\n Log: %s", shaderName.c_str(), infoLog);
    }

    for (GLuint shaderProg: shaders)
      glDeleteShader(shaderProg);
    debug_log("Shader %s was compiled.", shaderName.c_str());
    if (createShaders)
    {
      Shader(shaderName, program);
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