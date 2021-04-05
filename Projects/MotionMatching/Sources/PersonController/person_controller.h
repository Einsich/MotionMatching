#pragma once
#include "math.h"
#include "component.h"
class PersonController : public Component
{
private:
  void update(float dt);

public:
  float simulatedRotation, realRotation;
  vec3 speed, simulatedPosition, realPosition;
  PersonController(vec3 position);
  void update_from_speed(vec3 speed, float dt);
  void set_pos_rotation(vec3 position, float rotation);
};

