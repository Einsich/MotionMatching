#include "animation_tree_iterator.h"

AnimationTreeIterator::AnimationTreeIterator(int p, AnimationTree* tree)
:p(p), tree(tree){}
AnimationTreeIterator::AnimationTreeIterator(const AnimationTreeIterator &it)
:p(it.p), tree(it.tree){}
bool AnimationTreeIterator::operator!= (const AnimationTreeIterator& it)
{
  return it.tree != tree ? true : it.p != p;
}
typename AnimationTreeIterator::reference AnimationTreeIterator::operator*() const
{
  return tree->nodes[p];
}
typename AnimationTreeIterator::reference AnimationTreeIterator::operator->() const
{
  return tree->nodes[p];
}
AnimationTreeIterator& AnimationTreeIterator::operator++()
{
  p++;
  return *this;
}
int AnimationTreeIterator::index() const
{
  return p;
}
