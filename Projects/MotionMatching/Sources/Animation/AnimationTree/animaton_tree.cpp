#include "animation_tree.h"

#include <cmath>


AnimationNode::AnimationNode(const AnimationNodeData &data):
data(data){}

mat4 AnimationNode::get_transform() const
{
  return translation * rotation;
}
const string &AnimationNode::get_name() const
{
  return data.name;
}
const AnimationNodeData &AnimationNode::get_data() const
{
  return data;
}
int AnimationNode::parent() const
{
  return data.parent;
}


AnimationTree::AnimationTree(const AnimationTreeData &tree_data):
  data(tree_data), transforms(tree_data.nodes.size(), mat4(1.f))
{
  for (uint i = 0; i < tree_data.nodes.size(); i++)
  {    
    nodes.push_back(AnimationNode(data.nodes[i]));
  }
}
int AnimationTree::get_child(const string& name) const
{
  return data.get_child(name);
}

void AnimationTree::set_cadr(const AnimationCadr &cadr)
{
  for (uint i = 0; i < nodes.size(); i++)
  {
    const AnimationNodeData& node = data.nodes[i];
    mat4 rotation = glm::toMat4(cadr.nodeRotation[i]);
    mat4 translation =  glm::translate(mat4(1.f), (node.name == "Hips") ? cadr.nodeTranslation : node.translation);
    nodes[i].translation = translation;
    nodes[i].rotation = rotation;
  }
}
void AnimationTree::calculate_bone_transforms()
{
  for (uint i = 0; i < nodes.size(); i++)
  {
    const mat4 &parent = nodes[i].parent() >= 0 ? transforms[nodes[i].parent()] : mat4(1.f);
    transforms[i] = parent * nodes[i].get_transform() * data.nodes[i].avatarTransform;
  }

}
mat4 AnimationTree::get_bone_transform(int node) const
{
  return transforms[node] * data.nodes[node].meshToBone;;
}
mat4 AnimationTree::get_transform(int node) const
{
  return transforms[node];
}

const AnimationTreeData &AnimationTree::get_original_tree() const
{
  return data;
}