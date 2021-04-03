#include "animation_database.h"

AnimationDataBase::AnimationDataBase(aiNode *root):
tree(root), featureWeights(make_shared<AnimationFeaturesWeights>())
{
  add_tests();
}
int AnimationDataBase::cadr_count() const
{
  int count = 0;
  for (const AnimationClip & anim: clips)
    count += anim.duration;
  return count;
}
size_t AnimationDataBase::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, clips);
  return size;
}
size_t AnimationDataBase::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, clips);
  return size;
}
void AnimationDataBase::save_runtime_parameters()
{
  save_object(*featureWeights, "FeaturesWeights.bin");
}
void AnimationDataBase::load_runtime_parameters()
{
  load_object(*featureWeights, "FeaturesWeights.bin");
}
constexpr float dt = 0.2f;

constexpr vec3 hipsPoint = vec3(0, 0.96f, 0);
AnimationTest walk_test(vec3 dir, float speed, float max_t)
{
  dir = normalize(dir);
  AnimationTest test;
  float a = speed;
  float s = 0;
  float t = 0;
  vec3 p = hipsPoint;
  float stop_time = max_t * 0.8f;
  for (; t <= stop_time; t+= dt)
  {
    s = glm::clamp(s + a * dt, 0.f, speed);
    p += dir * s * dt;
    test.points.push_back({p, 0, t});
  }
  for (; t <= max_t; t+= dt)
  {
    s = glm::clamp(s - a * dt, 0.f, speed);
    p += dir * s * dt;
    test.points.push_back({p, 0, t});
  }
  return test;
}
void AnimationDataBase::add_tests()
{
  constexpr float walkSpeed = 1.2f;
  constexpr float runSpeed = walkSpeed * 2.f;

  for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++)
    {
      float c = j == 1 ? 1 : j == 0 ? 0.9f : 0.8f;
      test.push_back(walk_test(vec3(i, 0, j), walkSpeed * c, 5));
      test.push_back(walk_test(vec3(i, 0, j), runSpeed * c, 5));
    }
}

