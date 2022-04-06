#pragma once
#include "3dmath.h"

struct BoundingBox
{
  vec3 minBound, maxBound;
  vec3 diagonal() const
  {
    return maxBound - minBound;
  }
  vec3 center() const
  {
    return (maxBound + minBound) * 0.5f;
  }
  void minimize(vec3 p)
  {
    minBound = glm::min(minBound, p);
  }
  void maximize(vec3 p)
  {
    maxBound = glm::max(maxBound, p);
  }
};

