#pragma once
#include "CommonCode/math.h"
#include "CommonCode/component.h"
#include "CommonCode/Transform/transform.h"
#include "CommonCode/Physics/Colliders/collider.h"
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