#pragma once
#include <3dmath.h>

struct WorldRenderer : ecs::Singleton
{
  ivec2 resolution;
  mat3 screenToWorld;
  vec2 screen_to_world(int x, int y) const
  {
    vec3 screenPos(x*2 - resolution.x, resolution.y - y*2, 1);
    return screenToWorld * screenPos;
  }
};