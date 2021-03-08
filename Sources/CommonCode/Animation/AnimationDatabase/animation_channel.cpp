#include "animation_channel.h"
#include <assert.h>

AnimationChannel::AnimationChannel(uint duration, std::vector<vec3> & translations, std::vector<quat> & rotations):
  translation(translations), rotation(rotations)
{
  assert(translation.size() == 1 || translation.size() == duration);
  assert(rotation.size() == 1 || rotation.size() == duration);

}
vec3& AnimationChannel::get_translation(uint i)
{
  return translation.size() == 1 ? translation[0] : translation[i];
}
quat& AnimationChannel::get_rotation(uint i)
{
  return rotation.size() == 1 ? rotation[0] : rotation[i];
}
vec3 AnimationChannel::get_translation_c(uint i) const
{
  return translation.size() == 1 ? translation[0] : translation[i];
}
quat AnimationChannel::get_rotation_c(uint i) const
{
  return rotation.size() == 1 ? rotation[0] : rotation[i];
}
uint AnimationChannel::translation_size() const
{
  return translation.size();
}
uint AnimationChannel::rotation_size() const
{
  return rotation.size();
}
size_t AnimationChannel::serialize(std::ostream& os) const
{
  size_t s = 0;
  s += write(os, translation);
  s += write(os, rotation);
  return s;
}
size_t AnimationChannel::deserialize(std::istream& is)
{
  size_t s = 0;
  s += read(is, translation);
  s += read(is, rotation);
  return s;
}