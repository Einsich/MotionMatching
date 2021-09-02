#pragma once
#include "common.h"
#include <map>
#include <vector>
#include <filesystem>
#include "Serialization/reflection.h"
namespace fs = filesystem;
struct PrecompiledShader
{
  REFLECT(PrecompiledShader,
  (fs::file_time_type) (lastCompilation),
  (vector<uint8_t>) (data))
};
struct ShaderFileDependency
{
  REFLECT(ShaderFileDependency,
  (fs::file_time_type) (lastCompilationTime),
  (vector<fs::path>) (dependencies))
  bool valid = false, exists = false;
  //path my_path
  //vector<path> dependencies;
  //bool loaded
};

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