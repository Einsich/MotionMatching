#pragma once
#include "3dmath.h"
class Collider;

class Collision
{
public:
  float distance;
  vec3 collisionPoint, collisionNormal;
  std::shared_ptr<Collider> collider;
  Collision():
  distance(INFINITY), collisionPoint(vec3(NAN)), collisionNormal(vec3(NAN)), 
  collider(nullptr)
  {}
};

