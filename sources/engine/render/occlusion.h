#pragma once
#include "3dmath.h"

struct BoundingBox
{
  union 
  {
    vec3 box[2];
    vec3 minBound, maxBound;
  };
};

