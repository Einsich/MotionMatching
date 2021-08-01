#pragma once
#include "Shader/shader.h"
#include "Texture/textures.h"
#include "vertex_array_object.h"
#include "Serialization/reflection.h"
class SkyBox
{
public:
  Shader skyboxShader;
  VertexArrayObject skyboxVAO;
  CubeMap skybox;
  REFLECT(
    SkyBox,
    (string) (path)
  )
  SkyBox(string path);
  SkyBox() = default;
  void render(const mat4 view_projection, const vec3 &camera_position, bool wire_frame = false);
};