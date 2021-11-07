#pragma once
#include "3dmath.h"
#include "serialization/reflection.h"
class Shader;
class DirectionLight
{
public:
  REFLECT(DirectionLight,
  (vec3) (lightDirection),
  (vec3) (lightColor),
  (vec3) (ambient))
  vec3 normalizedLightDirection;

  DirectionLight() = default;
};