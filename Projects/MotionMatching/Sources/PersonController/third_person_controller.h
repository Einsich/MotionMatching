#pragma once
#include "component.h"
#include "math.h"
#include "Event/input.h"
class ThirdPersonController: public Component, public IUpdatable
{
private:
  vec3 wantedCameraOrientation, currentCameraOrientation;
  vec2 wantedCameraRotation, currentCameraRotation;
  float wantedRotation, currentRotation;
  float currentZoom, wantedZoom;
  bool crouching, disableEvents;
  float walkSpeed, runSpeedUp;
  float view_offset;
public:
  ThirdPersonController(vec2 rotation, float zoom, float walk_speed, float run_speed_up);
  void update() override;
  void mouse_move_handler(const MouseMoveEvent &e);
  void mouse_wheel_handler(const MouseWheelEvent &e);
  void view_offset_handler(const KeyboardEvent &e);
  void disable_events_handler(const KeyboardEvent &e);
};