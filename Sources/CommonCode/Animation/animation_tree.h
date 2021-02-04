#pragma once
#include "CommonCode/common.h"
#include "CommonCode/math.h"
#include <vector>
#include <map>
#include <assimp/scene.h>
#include "CommonCode/Serialization/serialization.h"
#include "animation_tree_iterator.h"
class AnimationNode: public ISerializable
{
public:
  string name;
  mat4 transform;
  mat4 animationTransform;
  mat4 meshToBone;
  int parent;
  vector<int> childs;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
class AnimationTreeIterator;

class AnimationTree: public ISerializable
{
private:
  map<string, int> childMap;
  void build_tree(aiNode *node, mat4 m, int index, int parent);
public:
  vector<AnimationNode> nodes;
  AnimationTree(){}
  AnimationTree(aiNode * node);
  int get_child(const string& name) const;
  
  AnimationTreeIterator begin() const;
  AnimationTreeIterator end() const;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};