#pragma once
#include "math.h"
class AnimationPlayer;
class Transform;
class PersonController
{
public:
  float simulatedRotation, realRotation, wantedRotation;
  vec3 speed, simulatedPosition, realPosition;
  bool disableEvents, crouching;
  PersonController(vec3 position);
  void update_from_speed(const AnimationPlayer &player, Transform &transform, vec3 speed, float dt);
  void set_pos_rotation(Transform &transform, vec3 position, float rotation);
};
class KeyboardEvent;
class MouseMoveEvent;
struct ControllerKeyBoardEvent
{
  KeyboardEvent e;
  ControllerKeyBoardEvent(const KeyboardEvent &e):e(e){}
};
struct ControllerMouseMoveEvent
{
  MouseMoveEvent e;
  ControllerMouseMoveEvent(const MouseMoveEvent &e):e(e){}
};