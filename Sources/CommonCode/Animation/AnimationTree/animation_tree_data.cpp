#include "animation_tree_data.h"


AnimationNodeData::AnimationNodeData(string && name, const mat4 &transform,  const mat4 &meshToBone, int parent):
  name(name), transform(transform), meshToBone(meshToBone), parent(parent), rotation(transform), translation(transform[3]){}


AnimationTreeData::AnimationTreeData(aiNode * node)
{
  build_tree(node, mat4(1.f), 0, -1);
  vector<mat4> worldTransforms(nodes.size());
  for (uint i = 0; i < nodes.size(); i++)
  {
    mat4 parent = nodes[i].parent >= 0 ? worldTransforms[nodes[i].parent] : mat4(1.f);
    worldTransforms[i] = parent * nodes[i].transform;
  }
  for (uint i = 0; i < nodes.size(); i++)
  {
    mat4 parent = nodes[i].parent >= 0 ? worldTransforms[nodes[i].parent] : mat4(1.f);
    nodes[i].worldPosition = parent * nodes[i].transform[3];
  }
  for (int i = nodes.size() - 1; i >= 0; i--)
    if (nodes[i].parent >= 0)
      nodes[i].localPosition = nodes[i].worldPosition - nodes[nodes[i].parent].worldPosition;
}

void AnimationTreeData::build_tree(aiNode *node, mat4 parent_transform, int index, int parent)
{
  mat4 transform;
  for (uint j = 0 ;j < 4; j++)
    transform[j] = to_vec4(node->mTransformation[j]);
  transform = transpose(transform);
  parent_transform = parent_transform * transform ;
  nodes.push_back(AnimationNodeData(string(node->mName.C_Str()), transform, inverse(parent_transform), parent));
  childMap[nodes[index].name] = index;
  for (uint i = 0; i < node->mNumChildren; i++)
  {
    int child = nodes.size();
    nodes.back().childs.push_back(child);
    build_tree(node->mChildren[i], parent_transform, child, index);
  }
}
int AnimationTreeData::get_child(const string& name) const
{
  auto it = childMap.find(name);
  return it == childMap.end() ? -1 : it->second;
}