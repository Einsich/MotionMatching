#pragma once
#include "vertex_array_object.h"
#include "shader/shader.h"
class PostFX
{
private:
  VertexArrayObject vao;
public:
  Shader shader;
  PostFX(Shader shader);
  void render(); 
};