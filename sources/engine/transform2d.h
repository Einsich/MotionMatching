#pragma once
#include "3dmath.h"
#include "serialization/reflection.h"
struct Transform2D
{
  REFLECT(Transform2D,
  (vec2) (position),
  (vec2) (scale),
  (float) (rotation))
  
  Transform2D(vec2 position = vec2(0.f), vec2 scale = vec2(1.f), float rotation = 0):
    position(position), scale(scale), rotation(rotation)
  {}
  mat3 get_matrix() const;
};