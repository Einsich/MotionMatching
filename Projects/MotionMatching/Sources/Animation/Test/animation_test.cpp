#include "animation_test.h"

AnimationTest::AnimationTest(const std::vector<TestPoint> &p):
  points(std::move(p))
{
}

TestPoint AnimationTest::get_lerped_point(int index, float time)
{
  while (index + 1 < (int)points.size() && points[index + 1].time <= time)
    index++;
  if (index + 1 >= (int)points.size())
    return points.back();
  const TestPoint &p0 = points[index];
  const TestPoint &p1 = points[index +1];
  float t = (time - p0.time) / (p1.time - p0.time);
  return {lerp(p0.point, p1.point, t), lerp(p0.velocity, p1.velocity, t), lerp(p0.rotation, p1.rotation, t), time};
}