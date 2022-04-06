#include "postfx.h"

PostFX::PostFX(Shader shader):
shader(shader)
{
  static Mesh postFXvao;
  if (!postFXvao.is_valid())
  {
    vector<uint> indices = {0,1,2,0,2,3};
    vector<vec2> corners =  {vec2(-1, -1), vec2(1,-1), vec2(1,1),vec2(-1,1)};
    postFXvao = Mesh(indices, corners);
    postFXvao.clear_cpu_data();
  }
  mesh = postFXvao;
}
void PostFX::render()
{
  mesh.render(false);
}