#include "animation_cadr.h"

size_t AnimationCadr::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, rootLinearVelocity);
  size += write(os, rootAngularVelocity);
  size += write(os, nodeRotation);
  size += write(os, nodeTranslation);
  return size;
}
size_t AnimationCadr::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, rootLinearVelocity);
  size += read(is, rootAngularVelocity);
  size += read(is, nodeRotation);
  size += read(is, nodeTranslation);
  return size;
}
AnimationCadr lerped_cadr(const AnimationCadr& cadr1, const AnimationCadr& cadr2, float t)
{
  AnimationCadr cadr;
  cadr.rootAngularVelocity = glm::mix(cadr1.rootAngularVelocity, cadr2.rootAngularVelocity, t);
  cadr.rootLinearVelocity = glm::mix(cadr1.rootLinearVelocity, cadr2.rootLinearVelocity, t);
  cadr.nodeTranslation = glm::mix(cadr1.nodeTranslation, cadr2.nodeTranslation, t);
  cadr.nodeRotation.resize(cadr1.nodeRotation.size());
  for (uint i = 0; i < cadr.nodeRotation.size(); i++)
  {
    cadr.nodeRotation[i] = glm::mix(cadr1.nodeRotation[i], cadr2.nodeRotation[i], t);
  }
  return cadr;
}