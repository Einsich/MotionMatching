#pragma once
#include "array2d.h"
#include <3dmath.h>
#include <set>
inline bool operator<(const ivec2&a, const ivec2&b)
{
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}
struct MapArrays
{
  struct cmp
  {
    ivec2 center;
    cmp(ivec2 center):
    center(center)
    { }
    inline uint64_t ilength2(const ivec2&a) const
    {
      return a.x*a.x+a.y*a.y;
    }
    bool operator()(const ivec2&a, const ivec2&b) const
    {
      uint64_t l1 = ilength2(center - a);
      uint64_t l2 = ilength2(center - b);
      return l1 == l2 ? (a.x == b.x ? a.y < b.y : a.x < b.x) : l1 < l2;
      return a.x == b.x ? a.y < b.y : a.x < b.x;
    }
  };
  array2d<uint> color_indices;
  std::vector<std::set<ivec2, cmp>> borders;
};