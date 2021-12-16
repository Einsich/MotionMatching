#pragma once
#include "../AnimationDatabase/animation_feature.h"

struct VPTree
{
  using T = FrameFeature;
  struct Node
  {
    const T *p;
    uint clip, frame;
    float bound;
  }; 
  AnimationTags tag;
  std::vector<Node> points; 
  std::function<float(const T&, const T&)> norma;
  VPTree(AnimationTags tag, std::vector<Node> &&points, std::function<float(const T&, const T&)> &&norma);
  std::pair<uint, uint> find_closect(const T &point) const;
};
