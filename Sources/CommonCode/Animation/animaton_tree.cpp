#include "animation_tree.h"

AnimationTree::AnimationTree(aiNode * node)
{
  nodes.push_back(AnimationNode());
  build_tree(node, mat4(1.f), 0, -1);
}

void AnimationTree::build_tree(aiNode *node, mat4 parent_transform, int index, int parent)
{
  nodes[index].parent = parent;
  for (int j = 0 ;j < 4; j++)
    nodes[index].transform[j] = to_vec4(node->mTransformation[j]);
  nodes[index].transform = transpose(nodes[index].transform);
  parent_transform = parent_transform * nodes[index].transform ;
  //if (index == 0)
  //  nodes[index].transform = mat4(mat3(nodes[index].transform));
  nodes[index].meshToBone = inverse(parent_transform);
  
  nodes[index].name = string(node->mName.C_Str());
  childMap[nodes[index].name] = index;
  for (uint i = 0; i < node->mNumChildren; i++)
  {
    int childIndex = nodes.size(); 
    nodes[index].childs.push_back(childIndex);
    nodes.push_back(AnimationNode());
    build_tree(node->mChildren[i], parent_transform, childIndex, index);
  }
}
size_t AnimationNode::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, name);
  size += write(os, transform);
  size += write(os, meshToBone);
  size += write(os, parent);
  size += write(os, childs);
  return size;
}
size_t AnimationNode::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, name);
  size += read(is, transform);
  size += read(is, meshToBone);
  size += read(is, parent);
  size += read(is, childs);
  return size;
}
size_t AnimationTree::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, nodes);
  return size;
}
size_t AnimationTree::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, nodes);
  return size;
}
AnimationTreeIterator AnimationTree::begin() const
{
  return AnimationTreeIterator(0, (AnimationTree*)this);
}
AnimationTreeIterator AnimationTree::end() const
{
  return AnimationTreeIterator(nodes.size(), (AnimationTree*)this);
}

int AnimationTree::get_child(const string& name)
{
  auto it = childMap.find(name);
  return it == childMap.end() ? -1 : it->second;
}