#pragma once
#include "math.h"
#include "Transform/transform.h"
#include "Physics/Colliders/collider.h"
class RigidBody
{
private:
  Transform *transform;
  Collider *collider;
  bool movable;
  float mass;
  vec3 V, F;
public:
  RigidBody(Transform *transform, Collider *collider, float movable, float mass, vec3 V = vec3(0.f), vec3 F = vec3(0.f));
};