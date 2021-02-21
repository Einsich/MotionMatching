#pragma once
#include "CommonCode/math.h"
#include "../Colliders/collider.h"
class RigidBody
{
  float mass;
  vec3 F, V;
  Collider *collider;
};