#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"
class AnimationPathFeature : public ISerializable
{
public: 
  static const int PathLength = 5;
  static const int SkipCadres = 4;
  float rotation;
  vector<vec3> path;
  AnimationPathFeature():
  rotation(0), path(PathLength){}
  virtual size_t serialize(std::ostream& os) const override
  {
    size_t size = 0;
    size += write(os, rotation);
    size += write(os, path);
    return size;
  }
  virtual size_t deserialize(std::istream& is) override
  {
    size_t size = 0;
    size += read(is, rotation);
    size += read(is, path);
    return size;
  }
};