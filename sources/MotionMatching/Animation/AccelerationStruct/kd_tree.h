#pragma once
#include "../AnimationDatabase/animation_feature.h"

struct KdTree
{
  using T = FrameFeature;
  struct Node
  {
    const T *p;
    uint clip, frame;
    //float nearestNeighbor;
    float get(uint dimension) const
    {
      return ((const float *)p)[dimension];
    }
  }; 
  AnimationTags tag;
  std::vector<Node> points;
  std::vector<float> weights;
  std::vector<int> weightsId;
  std::function<float(const T&, const T&)> norma;
  
  KdTree(const MotionMatchingSettings &settings, AnimationTags tag, std::vector<Node> &&points, std::function<float(const T&, const T&)> &&norma);
  std::pair<uint, uint> find_closest(const T &point, float tolerance_error) const;
};
