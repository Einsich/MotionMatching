#pragma once
#include "3dmath.h"
#include "Serialization/reflection.h"
class ThirdPersonController
{
public:
  REFLECT(ThirdPersonController,
  (float) (minZoom),
  (float) (maxZoom),
  (float) (zoomStrength),
  (float) (lerpStrength)
  )
  vec3 wantedCameraOrientation, currentCameraOrientation;
  vec2 wantedCameraRotation, currentCameraRotation;
  float currentZoom, wantedZoom;
  float view_offset;
  ThirdPersonController() = default;
  ThirdPersonController(vec2 rotation, float zoom):
    wantedCameraOrientation(0),
    currentCameraOrientation(0), 
    wantedCameraRotation(rotation),
    currentCameraRotation(rotation), 
    currentZoom(zoom),
    wantedZoom(zoom),
    view_offset(0)
    {}

};
