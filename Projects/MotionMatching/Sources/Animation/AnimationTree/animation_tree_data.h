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
  void apply_transforms();
public:
  vector<AnimationNodeData> nodes;
  AnimationTreeData(aiNode * node);
  AnimationTreeData() = default;
  int get_child(const string& name) const;
};