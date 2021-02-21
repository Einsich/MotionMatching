#pragma once
#include "collision.h"
#include "CommonCode/Transform/transform.h"
class Collider
{
public:
  const Transform *transform;
  Collider(const Transform *transform) :transform(transform){}

};
class PlaneCollider : public Collider
{
public:
  PlaneCollider(const Transform *transform) : Collider(transform){}
};
class SphereCollider : public Collider
{
public:
  float radius;
  SphereCollider(const Transform *transform) : Collider(transform){}
};
class BoxCollider : public Collider
{
public:
  vec3 scale;
  BoxCollider(const Transform *transform) : Collider(transform){}
};
class CapsuleCollider : public Collider
{
public:
  vec3 scale;
  CapsuleCollider(const Transform *transform) : Collider(transform){}
};
