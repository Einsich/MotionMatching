#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include "animation_tree_data.h"
#include "../AnimationDatabase/animation_cadr.h"

class AnimationNode
{
  const AnimationNodeData &data;
public:
  mat4 translation, rotation;
  AnimationNode(const AnimationNodeData &data);
  mat4 get_transform() const;
  const AnimationNodeData &get_data() const;
  const string &get_name() const;
  int parent() const;
};

class AnimationTree
{
private:
  const AnimationTreeData &data;
  vector<mat4> transforms;
public:
  vector<AnimationNode> nodes;
  AnimationTree(const AnimationTreeData &tree_data);
  int get_child(const string& name) const;
  void set_cadr(const AnimationCadr &cadr);
  void calculate_bone_transforms();
  mat4 get_bone_transform(int node) const;
  mat4 get_transform(int node) const;
  const AnimationTreeData & get_original_tree() const;
};
using AnimationTreePtr = shared_ptr<AnimationTree>;