#pragma once
#include "mesh.h"
#include "shader/shader.h"
class PostFX
{
private:
  Mesh mesh;
public:
  Shader shader;
  PostFX(Shader shader);
  void render(); 
};