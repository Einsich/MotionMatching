
#include <glm/glm.hpp>

using namespace glm;

float mod_f(float x, float n)
{
  return x - (int)(x/n) * n;
}
vec2 mod_f(vec2 v, float n)
{
  return vec2(mod_f(v.x, n), mod_f(v.y, n));
}
vec3 mod_f(vec3 v, float n)
{
  return vec3(mod_f(v.x, n), mod_f(v.y, n), mod_f(v.z, n));
}
vec4 mod_f(vec4 v, float n)
{
  return vec4(mod_f(v.x, n), mod_f(v.y, n), mod_f(v.z, n), mod_f(v.w, n));
}