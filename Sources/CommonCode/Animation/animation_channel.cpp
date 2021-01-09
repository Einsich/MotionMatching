#include "animation_channel.h"

mat4 AnimationChannel::get_lerped_translation(uint i, float t)
{
  return (i + 1 < pos.size()) ? translate(mat4(1.f), lerp(pos[i], pos[i + 1], t)) : mat4(1.f);
}
mat4 AnimationChannel::get_lerped_rotation(uint i, float t)
{
  return (i + 1 < rot.size()) ? toMat4(glm::mix(rot[i], rot[i + 1], t)): mat4(1.f);
}
mat4 AnimationChannel::get_lerped_locked_translation(uint i, float t, vec3 & translation)
{
  mat4 m = (i + 1 < pos.size()) ? translate(mat4(1.f), lerp(pos[i], pos[i + 1], t)) : mat4(1.f);
  translation = vec3(m[3].x, 0, m[3].z);
  m[3].x = m[3].z = 0;
  return m;
}
mat4 AnimationChannel::get_lerped_locked_rotation(uint i, float t, float & rotation)
{
  mat4 m =  (i + 1 < rot.size()) ? toMat4(glm::mix(rot[i], rot[i + 1], t)): mat4(1.f);
  float x, y, z;
  glm::extractEulerAngleXYZ(m, x, y, z);
  rotation = x;
  x = 0;
  m = glm::eulerAngleXYZ(x, y, z);
  return m;
}
size_t AnimationChannel::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, pos);
  size += write(os, rot);
  return size;
}
size_t AnimationChannel::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, pos);
  size += read(is, rot);
  return size;
}