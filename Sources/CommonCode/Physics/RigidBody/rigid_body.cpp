#include "rigid_body.h"

RigidBody::RigidBody(Transform *transform, Collider *collider, float movable, float mass, vec3 V, vec3 F) :
transform(transform), collider(collider), movable(movable), mass(mass), V(V), F(F)
{
  
}