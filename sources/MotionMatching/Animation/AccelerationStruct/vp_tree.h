#pragma once

template<typename T, typename N>
struct VPTree
{
  N norma;
  struct Node
  {
    const T *p;
    float bound;
  };
  
  std::vector<const T*> points; 
  VPTree(std::vector<const T*> &&points, const N &norma):
    points(points), norma(norma)
  {

  }

};
