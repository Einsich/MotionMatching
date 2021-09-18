
#include "Shader/shader.h"
#include "ecs/ecs.h"
#include "direction_light.h"
void DirectionLight::bind_to_shader(const Shader & shader) const
{
  shader.set_vec3("LightDirection", normalizedLightDirection);
}
void DirectionLight::unbind_to_shader(const Shader & shader) const
{
  shader.set_vec3("LightDirection", vec3());
}
REG_TYPE(DirectionLight)