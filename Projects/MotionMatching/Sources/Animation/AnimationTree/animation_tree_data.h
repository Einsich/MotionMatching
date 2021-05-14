#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include <map>
#include <assimp/scene.h>
class AnimationNodeData
{
public:
  string name;
  mat4 avatarTransform;
  mat4 transform;
  mat4 meshToBone;
  int parent;
  quat rotation;
  vec3 translation;
  vec3 localPosition, worldPosition;
  vector<int> childs;
  AnimationNodeData(string && name, const mat4 &transform,  const mat4 &meshToBone, int parent);

};
class AnimationTreeData
{
private:
  map<string, int> childMap;
  void build_tree(aiNode *node, mat4 m, int index, int parent);
  void apply_sub_tree(int my_node, const AnimationTreeData &other_tree, int node, mat4 my_p=mat4(1.f), mat4 other_p=mat4(0.01f), mat4 real_p=mat4(1.f));
  void apply_transforms();
public:
  vector<AnimationNodeData> nodes;
  AnimationTreeData(aiNode * node);
  int get_child(const string& name) const;
  void apply_other_tree(const AnimationTreeData &other_tree);
};