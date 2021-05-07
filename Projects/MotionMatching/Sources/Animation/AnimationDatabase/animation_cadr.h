#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include "Serialization/serialization.h"

class AnimationCadr final : public ISerializable
{
public:
  AnimationCadr()=default;
  ~AnimationCadr()=default;
  vec3 rootTranslationDelta;
  float rootRotationDelta;

  vec3 nodeTranslation;
  vector<quat> nodeRotation;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

AnimationCadr lerped_cadr(const AnimationCadr& cadr1, const AnimationCadr& cadr2, float t);