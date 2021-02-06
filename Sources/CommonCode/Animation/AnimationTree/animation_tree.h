#pragma once
#include "CommonCode/common.h"
#include "CommonCode/math.h"
#include <vector>
#include "animation_tree_data.h"
#include "../animation_cadr.h"

class AnimationNode
{
  const AnimationNodeData &data;
  mat4 animationTransform;
public:
  AnimationNode(const AnimationNodeData &data, const mat4 &animationTransform);
  mat4 get_transform() const;
  void set_transform(const mat4& parent_transformation, const mat4& local_transformation);
  mat4 get_bone_transform() const;
  const string &get_name() const;
  int parent() const;
};

class AnimationTree
{
private:
  const AnimationTreeData &data;
public:
  vector<AnimationNode> nodes;
  AnimationTree(const AnimationTreeData &tree_data);
  int get_child(const string& name) const;
  void set_cadr(const AnimationCadr &cadr);
};
using AnimationTreePtr = shared_ptr<AnimationTree>;