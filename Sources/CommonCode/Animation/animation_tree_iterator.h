#pragma once
#include "animation_tree.h"

class AnimationNode;
class AnimationTree;
class AnimationTreeIterator : public std::iterator<std::input_iterator_tag, AnimationNode>
{
  friend class AnimationTree;
private:
  int p;
  AnimationTree *tree;
public:
  AnimationTreeIterator(int p, AnimationTree* tree);
  AnimationTreeIterator(const AnimationTreeIterator &it);
  bool operator!= (const AnimationTreeIterator& it);
  typename AnimationTreeIterator::reference operator*() const;
  typename AnimationTreeIterator::reference operator->() const;
  AnimationTreeIterator& operator++();
  int index() const;

};