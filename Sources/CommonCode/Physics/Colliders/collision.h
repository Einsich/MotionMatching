#pragma once
#include "CommonCode/math.h"
#include "CommonCode/GameObject/game_object.h"
class Collider;

class Collision
{
public:
  float distance;
  vec3 collisionPoint, collisionNormal;
  GameObjectPtr gameObject;
  std::shared_ptr<Collider> collider;
  Collision():
  distance(INFINITY), collisionPoint(vec3(NAN)), collisionNormal(vec3(NAN)), 
  gameObject(nullptr), collider(nullptr)
  {}
};

