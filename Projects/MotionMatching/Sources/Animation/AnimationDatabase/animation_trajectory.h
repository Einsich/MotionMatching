#pragma once
#include "common.h"
#include <array>
#include "Serialization/serialization.h"
#include <3dmath.h>
struct TrajectoryPoint final : public ISerializable
{
  vec3 point;
  float rotation;
  TrajectoryPoint();
  ~TrajectoryPoint()=default;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
class AnimationTrajectory final : public ISerializable
{
public: 
  static const int PathLength = 3;
  static constexpr float timeDelays[PathLength] = { 0.333f, 0.666f,1.f};
  array<TrajectoryPoint, PathLength> trajectory;
  AnimationTrajectory() = default;
  ~AnimationTrajectory() = default;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};