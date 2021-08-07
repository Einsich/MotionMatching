#pragma once
#include "3dmath.h"
#include "Serialization/reflection.h"
class Shader;
class DirectionLight
{
public:
  REFLECT(DirectionLight,
  (vec3) (lightDirection))
  vec3 normalizedLightDirection;

  DirectionLight() = default;
  void bind_to_shader(const Shader & shader) const;
  void unbind_to_shader(const Shader & shader) const;
};