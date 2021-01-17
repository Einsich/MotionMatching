#include "animation_channel.h"

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