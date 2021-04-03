#include "collider.h"
#include "common.h"
bool SphereCollider::raycast_hit(Collision &collision,const mat4 &transform, const mat3 &, const Ray &ray)
{
  vec3 center = ray.from - (vec3)transform[3];
  float c = dot(center, center) - radius * radius;
  float b = dot(center, ray.dir);
  float a = dot(ray.dir, ray.dir);
  float D = b * b - a * c;
  if (D < 0)
    return false;
  D = sqrt(D);
  float t = false;
  float t1 = (-D - b) / a;
  if (t1 >= 0)
    t = t1;
  else
  {
    float t2 = (D - b) / a;
    if (t2 >= 0)
      t = t2;
    else
      return false;
  }

  collision.distance = t;
  collision.collisionPoint = ray.from + ray.dir * t;
  collision.collisionNormal = normalize(collision.collisionPoint - (vec3)transform[3]);
  return true;
}
bool plane_raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray, int axis, bool side, vec3 scale, vec3 offset)
{
  vec3 corners[4] = {vec3(-1,-1,0), vec3(1,-1, 0), vec3(1,1, 0), vec3(-1,1,0)};

  for (uint i = 0; i < 4; ++i)
  {
    vec3 corner;
    corner[axis] = 0;
    corner[(axis + 1) % 3] = side ? corners[i].x : corners[i].y;
    corner[(axis + 2) % 3] = side ? corners[i].y : corners[i].x;
    corners[i] = (corner + offset) * scale;
  }
  offset = transform * vec4(offset * scale, 1);
  for (uint i = 0; i < 4; ++i)
    corners[i] = transform * vec4(corners[i], 1);
  vec3 normal(0.f);
  normal[axis] = side ? -1 : 1;
  
  normal = rotation * normal;

  //debug_log("[%f, %f, %f]", normal.x, normal.y, normal.z);
  float NaD = -dot(normal, ray.dir);
  if (NaD <= 1e-6)
    return false;
  float NaO = dot(ray.from - offset, normal);
  if (NaO < 0)
    return false;
  float t = NaO / NaD;
  vec3 p = ray.from + ray.dir * t;
  collision.distance = t;
  collision.collisionPoint = p;
  collision.collisionNormal = normal;

  vec3 crosses[4];
  for (uint i = 0; i < 4; ++i)
  {
    crosses[i] = cross(p -  corners[(i + 3) & 3], corners[i] - corners[(i + 3) & 3]);
  }
  float dot0 = dot(crosses[0], crosses[1]);
  for (uint i = 2; i < 4; i++)
    if (dot(crosses[i], crosses[(int)i - 1]) * dot0 < 0)
      return false;

  return true;
}
bool PlaneCollider::raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray)
{
  return plane_raycast_hit(collision, transform, rotation, ray, 1, false, vec3(1.f), vec3(0.f));
}
bool BoxCollider::raycast_hit(Collision &collision, const mat4 &transform, const mat3 &rotation, const Ray &ray)
{
  for (int face = 0; face < 3; face++)
  {
    vec3 norm = vec3();
    norm[face] = 1;
    if (plane_raycast_hit(collision, transform, rotation, ray, face, false, scale, norm) ||
        plane_raycast_hit(collision, transform, rotation, ray, face, true, scale, -norm))
      return true;
    
  }
  return false;
}
bool CapsuleCollider::raycast_hit(Collision &, const mat4 &, const mat3 &, const Ray &)
{
  return false;
}