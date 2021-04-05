#pragma once
#include "math.h"
#include "../animation_goal.h"
#include <vector>
struct TestPoint
{
  vec3 point, velocity;
  float rotation;
  float time;
};

class AnimationTest
{
public:
  std::vector<TestPoint> points;
  AnimationTest() = default;
  AnimationTest(const std::vector<TestPoint> &points);
  TestPoint get_lerped_point(int index, float time);
};