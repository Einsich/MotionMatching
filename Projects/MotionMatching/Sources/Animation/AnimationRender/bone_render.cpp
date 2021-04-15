#include "bone_render.h"
#include "Components/DebugTools/debug_arrow.h"


static vector<vec3> boneOffsets;

BoneRender::BoneRender()
{
}

void BoneRender::render(const mat4& transform, const AnimationTree &tree) const
{
  uint n = tree.nodes.size();
  if (boneOffsets.size() != n)
  {
    boneOffsets.resize(n);
  }
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    boneOffsets[i] = vec3(tree.get_transform(i)[3]);
  }
  mat4 flip = mat4(1.f);
  flip[0] = vec4(0,1,0,0);
  flip[1] = vec4(1,0,0,0);
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    vec3 p = tree.nodes[i].parent() >= 0 ? boneOffsets[tree.nodes[i].parent()] : vec3(0.f);
    float width = std::min(0.5f, length(boneOffsets[i] - p) * 0.05f);
    draw_arrow(transform, p, boneOffsets[i], vec3(0,0.8f,0), width);
  }
}