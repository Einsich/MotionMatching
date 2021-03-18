#pragma once
#include <memory>
#include "math.h"
#include "component.h"
#include "collision.h"
#include "../ray.h"
class Collider : public Component
{
public:
  virtual bool raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray) = 0;
};
class PlaneCollider : public Collider
{
public:
  PlaneCollider(){}
  bool raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray) override;
};
class SphereCollider : public Collider
{
public:
  float radius;
  SphereCollider(float radius) : radius(radius){}
  bool raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray) override;
};
class BoxCollider : public Collider
{
public:
  vec3 scale;
  BoxCollider(vec3 scale = vec3(1.f)) : scale(scale){}
  bool raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray) override;
};
class CapsuleCollider : public Collider
{
public:
  vec3 scale;
  CapsuleCollider(vec3 scale) : scale(scale){}
  bool raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray) override;
};

using ColliderPtr = std::shared_ptr<Collider>;

template<typename T, typename ...Args>
ColliderPtr make_collider(Args ... args)
{
  return std::shared_ptr<Collider>((Collider*)(new T(args...)));
}

