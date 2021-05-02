#pragma once
#include "common.h"
#include "3dmath.h"
#include "Shader/shader.h"
class DirectionLight
{
private:
  vec3 lightDirection;
public:
  DirectionLight();
  DirectionLight(vec3 lightDirection);
  void bind_to_shader(const Shader & shader) const;
  void unbind_to_shader(const Shader & shader) const;
};