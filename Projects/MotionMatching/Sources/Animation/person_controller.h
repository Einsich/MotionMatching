#pragma once
#include "3dmath.h"
#include "AnimationDatabase/animation_trajectory.h"
class AnimationPlayer;
class Transform;
class PersonController
{
public:
  float simulatedRotation, realRotation, wantedRotation, angularSpeed;
  vec3 desiredTrajectory[AnimationTrajectory::PathLength];
  float desiredOrientation[AnimationTrajectory::PathLength];
  vec3 speed, simulatedPosition, realPosition;
  bool disableEvents, crouching;
  int rotationStrafe;
  PersonController(vec3 position);
  PersonController() = default;
  void set_pos_rotation(Transform &transform, vec3 position, float rotation);
};
struct KeyboardEvent;
struct MouseMoveEvent;
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