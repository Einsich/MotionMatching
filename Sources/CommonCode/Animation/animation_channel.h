#pragma once
#include "CommonCode/common.h"
#include <vector>
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/math.h"

class AnimationCadr : public ISerializable
{
public:
  vec3 rootTranslationDelta;
  float rootRotationDelta;

  vec3 nodeTranslation;
  vector<quat> nodeRotation;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

AnimationCadr lerped_cadr(const AnimationCadr& cadr1, const AnimationCadr& cadr2, float t);