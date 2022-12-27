#pragma once
#include "3dmath.h"
#include "AnimationDatabase/animation_feature.h"
#include "serialization/reflection.h"
#include <ecs/event.h>
class AnimationPlayer;
class Transform;
class PersonController
{
public:
  struct PathPrediction
  {
    vec3 pos; float rot, time;
  };
  REFLECT(PersonController, 
  (bool) (fake_field))
  float simulatedRotation, realRotation, wantedRotation, angularSpeed;
  std::array<vec3, AnimationTrajectory::PathLength> desiredTrajectory;
  std::array<float, AnimationTrajectory::PathLength> desiredOrientation;
  std::queue<PathPrediction> path;
  vec3 speed, simulatedPosition, realPosition;
  bool disableEvents, crouching;
  int rotationStrafe;
  PersonController(vec3 position);
  PersonController() = default;
  void set_pos_rotation(Transform &transform, vec3 position, float rotation);
};
struct KeyEventAnyActionKey;
struct MouseMoveEvent;
struct ControllerKeyBoardEvent : public ecs::Event
{
  KeyEventAnyActionKey e;
  ControllerKeyBoardEvent(const KeyEventAnyActionKey &e):e(e){}
};
struct ControllerMouseMoveEvent : public ecs::Event
{
  MouseMoveEvent e;
  ControllerMouseMoveEvent(const MouseMoveEvent &e):e(e){}
};