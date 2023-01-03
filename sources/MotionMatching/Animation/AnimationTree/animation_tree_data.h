#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include <map>
#include <assimp/scene.h>
class AnimationNodeData
{
public:
  std::string name;
  mat4 avatarTransform;
  mat4 transform;
  mat4 meshToBone;
  int parent;
  quat rotation;
  vec3 translation;
  vec3 localPosition, worldPosition;
  std::vector<int> childs;
  AnimationNodeData(std::string && name, const mat4 &transform,  const mat4 &meshToBone, int parent);

};
class AnimationTreeData
{
private:
  std::map<std::string, int> childMap;
  void build_tree(aiNode *node, mat4 m, int index, int parent);
  void apply_transforms();
public:
  std::vector<AnimationNodeData> nodes;
  AnimationTreeData(aiNode * node);
  AnimationTreeData() = default;
  int get_child(const std::string& name) const;
};