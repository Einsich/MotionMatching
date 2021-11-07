#pragma once
#include "common.h"
#include <array>
#include "serialization/serialization.h"
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
  static constexpr std::array<float, PathLength> timeDelays = { 0.15f, 0.3f, 0.5f};
  array<TrajectoryPoint, PathLength> trajectory;
  AnimationTrajectory() = default;
  ~AnimationTrajectory() = default;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};