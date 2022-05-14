#pragma once
#include "../AnimationDatabase/animation_feature.h"

struct CoverTree
{
  using T = FrameFeature;
  struct Node
  {
    vector<uint> childs;
    const T *p;
    uint clip, frame;
    float nearestNeighbor, bound;
  }; 
  AnimationTags tag;
  std::vector<Node> points;
  float maxRadius; 
  std::function<float(const T&, const T&)> norma;
  CoverTree(AnimationTags tag, std::vector<Node> &&points, std::function<float(const T&, const T&)> &&norma);
  std::pair<uint, uint> find_closest(const T &point, float tolerance_erorr) const;
};
