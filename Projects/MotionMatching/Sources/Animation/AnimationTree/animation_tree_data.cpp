#include "animation_tree_data.h"

AnimationNodeData::AnimationNodeData(string && name, const mat4 &transform,  const mat4 &meshToBone, int parent):
  name(name), avatarTransform(1.f), transform(transform), meshToBone(meshToBone), parent(parent), rotation(transform), translation(transform[3])
{

}


AnimationTreeData::AnimationTreeData(aiNode * node)
{
  build_tree(node, mat4(1.f), 0, -1);
  apply_transforms();
}

void AnimationTreeData::build_tree(aiNode *node, mat4 parent_transform, int index, int parent)
{
  mat4 transform;
  for (uint j = 0 ;j < 4; j++)
    transform[j] = to_vec4(node->mTransformation[j]);
  transform = transpose(transform);
  parent_transform = parent_transform * transform ;
  string name(node->mName.C_Str());

 // printf("{ \"%s\", \"\"},%d\n", name.c_str(), parent);
  nodes.push_back(AnimationNodeData(move(name), transform, inverse(parent_transform), parent));
  childMap[nodes[index].name] = index;
  for (uint i = 0; i < node->mNumChildren; i++)
  {
    int child = nodes.size();
    nodes[index].childs.push_back(child);
    build_tree(node->mChildren[i], parent_transform, child, index);
  }
}
int AnimationTreeData::get_child(const string& name) const
{
  auto it = childMap.find(name);
  return it == childMap.end() ? -1 : it->second;
}
void AnimationTreeData::apply_transforms()
{
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

void AnimationTreeData::apply_other_tree(const AnimationTreeData &other_tree)
{
  apply_sub_tree(0, other_tree, 0);
  apply_transforms();
}

void AnimationTreeData::apply_sub_tree(int my_node, const AnimationTreeData &other_tree, int node, mat4 my_p, mat4 other_p, mat4 real_p)
{
  //if (my_node >2)
  //  return;
  AnimationNodeData &curNode = nodes[my_node];
  const AnimationNodeData &otherNode = other_tree.nodes[node];
  debug_log("%s %s", curNode.name.c_str(), otherNode.name.c_str());

  my_p = curNode.transform;
  other_p = otherNode.transform;
  if (node == 0)
    other_p *= 0.01f;

mat4 dd[4] = {other_p*inverse(my_p), inverse(other_p)*(my_p), my_p*inverse(other_p), inverse(my_p)*(other_p)};
  mat4 d = dd[1];
  //d[3]=vec4(0,0,0,1);
  log_mat4x4(my_p);
  log_mat4x4(other_p);
  log_mat4x4(d);
  

  curNode.avatarTransform = (d);
  
  for (int my_child : curNode.childs)
  {
    for (int other_child : otherNode.childs)
    {

      if (nodes[my_child].name == other_tree.nodes[other_child].name)
      {
        apply_sub_tree(my_child, other_tree, other_child, my_p, other_p, real_p);
        break;
      }
    }
    
  }
}