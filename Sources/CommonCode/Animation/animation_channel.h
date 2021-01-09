#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"
class AnimationChannel : public ISerializable
{
public:
  vector<vec3> pos;
  vector<quat> rot;
  mat4 get_lerped_translation(uint i, float t);
  mat4 get_lerped_rotation(uint i, float t);
  mat4 get_lerped_locked_translation(uint i, float t, vec3 & translation);
  mat4 get_lerped_locked_rotation(uint i, float t, float & rotation);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};