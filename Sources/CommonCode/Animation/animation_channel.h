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
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};