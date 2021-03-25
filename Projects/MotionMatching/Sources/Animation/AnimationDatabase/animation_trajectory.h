#pragma once
#include "common.h"
#include <array>
#include "Serialization/serialization.h"
#include "math.h"
struct TrajectoryPoint : public ISerializable
{
  vec3 point;
  quat rotation;
  float timeDelay;
  TrajectoryPoint();
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
class AnimationTrajectory : public ISerializable
{
public: 
  static const int PathLength = 4;
  static constexpr float timeDelays[PathLength] = {0.1f, 0.2f, 0.4f, 0.8f};
  static constexpr float weights[PathLength] = {0.4f, 0.3f, 0.2f, 0.1f};
  array<TrajectoryPoint, PathLength> trajectory;
  AnimationTrajectory() = default;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};