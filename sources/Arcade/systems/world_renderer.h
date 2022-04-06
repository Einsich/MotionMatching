#pragma once
#include <string>
#include <vector>
#include <render/texture/texture2d.h>
#include <render/shader/shader.h>
#include <singleton.h>

class WorldRenderer : ecs::Singleton
{
public:
  mat3 screenToWorld;
  ivec2 resolution;
  std::vector<Texture2D*> textures;
  Shader spriteShader;
  VertexArrayObject spriteVao;
  WorldRenderer();

  Texture2D *add_texture(Texture2D *texture);
  Texture2D *get_texture(const std::string &name);
  vec2 screen_to_world(int x, int y) const;
};