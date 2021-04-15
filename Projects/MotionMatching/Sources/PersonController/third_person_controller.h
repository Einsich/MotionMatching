#pragma once
#include "math.h"

class ThirdPersonController
{
public:
  vec3 wantedCameraOrientation, currentCameraOrientation;
  vec2 wantedCameraRotation, currentCameraRotation;
  float currentZoom, wantedZoom;
  bool crouching, disableEvents;
  float view_offset;
  ThirdPersonController(vec2 rotation, float zoom):
    wantedCameraOrientation(0),
    currentCameraOrientation(0), 
    wantedCameraRotation(rotation),
    currentCameraRotation(rotation), 
    currentZoom(zoom),
    wantedZoom(zoom),
    crouching(false),
    disableEvents(false),
    view_offset(0)
    {}

};
