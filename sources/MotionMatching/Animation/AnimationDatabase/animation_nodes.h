#pragma once

#define NODES\
  NODE(LeftHand)\
  NODE(RightHand)\
  NODE(LeftToeBase)\
  NODE(RightToeBase)
enum class AnimationFeaturesNode
{
#define NODE(node) node,
  NODES
  Count
#undef NODE
};