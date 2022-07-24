#pragma once
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
using namespace glm;

constexpr float PI = 3.1415926535897932384626433832795f;
constexpr float PIHALF = PI * 0.5f;
constexpr float PITWO = PI * 2.0f;
constexpr float DegToRad = PI / 180.f;
constexpr float RadToDeg = 1.f / DegToRad;
template<typename T>
vec2 to_vec2(const T& t)
{
  return vec2(t.x, t.y);
}
template<typename T>
vec3 to_vec3(const T& t)
{
  return vec3(t.x, t.y, t.z);
}
template<typename T>
vec4 to_vec4(const T& t)
{
  return vec4(t[0], t[1], t[2], t[3]);
}
template<typename T>
quat to_quat(const T& t)
{
  return quat(t.w, t.x, t.y, t.z);
}
#define float_equal(x, y) (abs((x) - (y)) < 1e-8f)

inline float mod_f(float x, float n)
{
  return x - (int)(x/n) * n;
}
inline vec2 mod_f(vec2 v, float n)
{
  return vec2(mod_f(v.x, n), mod_f(v.y, n));
}
inline vec3 mod_f(vec3 v, float n)
{
  return vec3(mod_f(v.x, n), mod_f(v.y, n), mod_f(v.z, n));
}
inline vec4 mod_f(vec4 v, float n)
{
  return vec4(mod_f(v.x, n), mod_f(v.y, n), mod_f(v.z, n), mod_f(v.w, n));
}


inline int rand_int(int max_val = RAND_MAX)
{
  return abs(rand()) % max_val;
}

inline float rand_float(float from = -1.f, float to = 1.f)
{
  return from + ((float)rand() / RAND_MAX) * (to - from);
}

inline vec2 rand_vec2(float from = -1.f, float to = 1.f)
{
  return vec2(rand_float(from, to), rand_float(from, to));
}

inline vec3 rand_vec3(float from = -1.f, float to = 1.f)
{
  return vec3(rand_float(from, to), rand_float(from, to), rand_float(from, to));
}

inline vec4 rand_vec4(float from = 0.f, float to = 1.f)
{
  return vec4(rand_float(from, to), rand_float(from, to), rand_float(from, to), rand_float(from, to));
}