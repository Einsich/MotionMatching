#pragma once
#include "CommonCode/component.h"
#include "CommonCode/math.h"
#include "CommonCode/Event/input.h"
class ThirdPersonController: public Component, public IUpdatable
{
private:
  vec3 wantedOrientation, currentOrientation;
  vec2 wantedRotation, currentRotation;
  bool crouching;
  float walkSpeed, runSpeedUp;
public:
  ThirdPersonController(vec2 rotation, float walk_speed, float run_speed_up);
  void update() override;
  void mouse_move_handler(const MouseMoveEvent &e);
};