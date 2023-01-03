#pragma once
#include "common.h"
#include <map>
#include <vector>
#include <filesystem>
#include "serialization/reflection.h"
namespace fs = std::filesystem;
struct PrecompiledShader
{
  REFLECT(PrecompiledShader,
  (std::string) (name),
  (std::vector<uint8_t>) (data),
  (uint) (format))
};

enum class ShaderLexema
{
  SHADER_NAME,
  VARIANT,
  VS_SHADER,
  PS_SHADER,
  INCLUDE
};
struct MatchRange
{
  std::string::iterator begin, end;
  int beginIndex, endIndex;
  ShaderLexema type;
  std::string typeContent;
  bool empty() const
  {
    return begin == end;
  }
  std::string str() const
  {
    return std::string(begin, end);
  }
};
struct ShaderFileDependency
{
  REFLECT(ShaderFileDependency,
  (fs::file_time_type) (lastCompilationTime),
  (std::vector<std::string>) (dependencies),
  (fs::path) (path),
  (bool) (isShaderFile))
  bool valid = false, exists = false, loaded = false;
  ;
  std::string content;
  std::vector<MatchRange> lexems;
  std::vector<std::pair<std::string, std::vector<int>>> intervals;
};

bool compile_shader(const std::string &shaderName, const std::vector<std::pair<GLuint, const char*>> &shaders, GLuint &program);
bool compile_shader(const std::string &shaderName, const std::vector<std::pair<GLuint, std::string>> &shaders, GLuint &program);


bool load_precompiled_shader(const PrecompiledShader &shader, GLuint &program);
void save_precompiled_shader(PrecompiledShader &shader, const GLuint &program);
void load_precompiled_shaders(const std::vector<PrecompiledShader> &shaders);
void save_precompiled_shaders(std::vector<PrecompiledShader> &shaders);

void update_file(const fs::path &file_path);

std::map<std::string, ShaderFileDependency> &getShaderFiles();
/*
Load-debug
load file dependencies+
analize folder+
read precompiled shaders
update precompiled shaders from src

Reload-debug
analize folder
update precompiled shaders from src

Save-debug
save file dependencies+
save precompiled shader

Load-release
read precompiled shaders

Reload-release
XXX
Save-release
XXX
  */