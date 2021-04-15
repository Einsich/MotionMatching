#pragma once
#include "common.h"
#include "Shader/shader.h"
#include "Texture/textures.h"
#include "Mesh/vertex_array_object.h"
class SkyBox
{
public:
  Shader skyboxShader;
  VertexArrayObject skyboxVAO;
  CubeMap skybox;
  SkyBox(string path);
  void render(const mat4 view_projection, const vec3 &camera_position, bool wire_frame = false);
};