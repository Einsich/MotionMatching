#pragma once 
#include <ecs/singleton.h>
#include <3dmath.h>

struct MapRenderData : ecs::Singleton
{
  struct Border
  {
    vec4 color0, color1;
  };
  enum {
    MAX_BORDERS = 2048
  };
  vec4 mapSize;
  Border borders[MAX_BORDERS];
};
