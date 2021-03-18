#pragma once
#include "math.h"

class Ray
{
public:
  const vec3 from, dir;
  const float max_dist;
  Ray(vec3 from, vec3 dir, float max_dist = INFINITY):
    from(from), dir(normalize(dir)), max_dist(max_dist)
    {}
};