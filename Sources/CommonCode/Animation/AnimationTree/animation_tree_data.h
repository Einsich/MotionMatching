#pragma once
#include "CommonCode/common.h"
#include "CommonCode/math.h"
#include <vector>
#include <map>
#include <assimp/scene.h>
class AnimationNodeData
{
public:
  const string name;
  const mat4 transform;
  const mat4 meshToBone;
  const int parent;
  vector<int> childs;
  AnimationNodeData(string && name, const mat4 &transform,  const mat4 &meshToBone, int parent);

};
class AnimationTreeData
{
private:
  map<string, int> childMap;
  void build_tree(aiNode *node, mat4 m, int index, int parent);
public:
  vector<AnimationNodeData> nodes;
  AnimationTreeData(aiNode * node);
  int get_child(const string& name) const;

};