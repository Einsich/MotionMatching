#pragma once
#include "3dmath.h"
#include "serialization/reflection.h"
class ThirdPersonController
{
public:
  REFLECT(ThirdPersonController,
  (float) (minZoom),
  (float) (maxZoom),
  (float) (zoomStrength),
  (float) (lerpStrength),
  (vec2) (currentCameraRotation),
  (vec2) (wantedCameraRotation)
  )
  vec3 wantedCameraOrientation, currentCameraOrientation;
  float currentZoom, wantedZoom;
  ThirdPersonController() = default;

};
