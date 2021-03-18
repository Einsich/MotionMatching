#pragma once

#include "math.h"
#include <vector>
#include "Serialization/serialization.h"
class AnimationChannel : ISerializable
{
  std::vector<vec3> translation;
  std::vector<quat> rotation;
public:
  AnimationChannel(){}
  AnimationChannel(uint duration, std::vector<vec3> & translations, std::vector<quat> & rotation);
  vec3& get_translation(uint i);
  quat& get_rotation(uint i);
  vec3 get_translation_c(uint i) const;
  quat get_rotation_c(uint i) const;
  uint translation_size() const;
  uint rotation_size() const;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};