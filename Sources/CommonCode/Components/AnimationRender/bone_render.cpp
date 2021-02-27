#include "bone_render.h"
#include "CommonCode/Components/DebugTools/debug_arrow.h"

BoneRender::BoneRender()
{
}

void BoneRender::calculate_transforms(const mat4& transform, const AnimationTree &tree)
{
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
void BoneRender::render(const mat4& transform, const AnimationTree &tree)
{
  uint n = tree.nodes.size();
  if (boneOffsets.size() != n)
  {
    boneOffsets.resize(n);
  }
  calculate_transforms(transform, tree);
}