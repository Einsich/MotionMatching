#pragma once
#include "../AnimationDatabase/animation_feature.h"

struct VPTree
{
  using T = FrameFeature;
  struct Node
  {
    const T *p;
    float bound;
  }; 
  std::vector<const T*> points; 
  std::function<float(const T&, const T&)> norma;
  VPTree(std::vector<const T*> &&points, std::function<float(const T&, const T&)> &&norma):
    points(points), norma(norma)
  {

  }

};
