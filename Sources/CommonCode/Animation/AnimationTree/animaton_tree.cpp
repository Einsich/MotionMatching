#include "animation_tree.h"




AnimationNode::AnimationNode(const AnimationNodeData &data, const mat4 &animationTransform):
data(data), animationTransform(animationTransform){}

mat4 AnimationNode::get_transform() const
{
  return animationTransform;
}
void AnimationNode::set_transform(const mat4& parent_transformation, const mat4& local_transformation)
{
  animationTransform = parent_transformation * local_transformation;
}
mat4 AnimationNode::get_bone_transform() const
{
  return animationTransform * data.meshToBone;
}
const string &AnimationNode::get_name() const
{
  return data.name;
}
int AnimationNode::parent() const
{
  return data.parent;
}


AnimationTree::AnimationTree(const AnimationTreeData &tree_data):
  data(tree_data)
{
  for (uint i = 0; i < tree_data.nodes.size(); i++)
  {
    mat4 parent = data.nodes[i].parent < 0 ? mat4(1.f) : nodes[data.nodes[i].parent].get_transform();
    
    nodes.push_back(AnimationNode(data.nodes[i], parent * data.nodes[i].transform));
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
    mat4 transform = node.transform;
    mat4 parent = node.parent >= 0 ? nodes[node.parent].get_transform() : mat4(1.f);
    mat4 rotation = glm::toMat4(cadr.nodeRotation[i]);
    mat4 translation = (node.name == "Hips") ? glm::translate(mat4(1.f), cadr.nodeTranslation) * mat4(mat3(transform)) : transform;
    nodes[i].set_transform(parent, translation * rotation);
    
  }
}