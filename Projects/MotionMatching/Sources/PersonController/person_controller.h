#pragma once
#include "math.h"
class AnimationPlayer;
class Transform;
class PersonController
{
public:
  float simulatedRotation, realRotation;
  vec3 speed, simulatedPosition, realPosition;
  PersonController(vec3 position);
  void update_from_speed(const AnimationPlayer &player, Transform &transform, vec3 speed, float dt);
  void set_pos_rotation(Transform &transform, vec3 position, float rotation);
};

