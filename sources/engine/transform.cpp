#include "transform.h"
#include <type_registration.h>

ECS_REGISTER_TYPE_AND_VECTOR(Transform, Transform, false, true);

Transform::Transform(vec3 position, mat4x4 rotation, vec3 scale):
  position(position), rotation(rotation), scale(scale), calculated(false)
{}
Transform::Transform(vec3 position, vec3 rotation, vec3 scale):
  position(position), rotation(glm::yawPitchRoll(rotation.x, rotation.y, rotation.z)), scale(scale), calculated(false)
{}

Transform::Transform():
position(vec3(0.f)), rotation(mat4x4(1.f)), scale(vec3(1.f)), calculated(false)
{}
vec3& Transform::get_position()
{
  return position;
  calculated = false;
}
const vec3& Transform::get_position() const 
{
  return position;
}
const mat4x4 &Transform::get_transform() const
{
  if (calculated)
    return cachedTransform;
  else
  {
    calculated = true;
    cachedTransform = glm::translate(mat4(1.f), position) * glm::scale(rotation, scale);
    cached3x4Transform = transpose(cachedTransform);
    return cachedTransform;
  }
}
const mat3x4 &Transform::get_3x4transform() const
{
  if (!calculated)
    get_transform();
  return cached3x4Transform;
}
mat3 Transform::get_rotation() const
{
  return rotation;
}
vec3 Transform::get_forward() const
{
  return -rotation[2];
}
vec3 Transform::get_right() const
{
  return rotation[0];
}
vec3 Transform::get_up() const
{
  return rotation[1];
}
vec3 Transform::get_scale() const
{
  return scale;
}
void Transform::set_rotation(float yaw, float pitch, float roll)
{
  rotation = glm::yawPitchRoll(yaw, pitch, roll);
  calculated = false;
}
void Transform::set_rotation(const mat4& rotation)
{
  this->rotation = rotation;
  calculated = false;
}
void Transform::set_position(const vec3 position)
{
  this->position = position;
  calculated = false;
}
void Transform::set_scale(const vec3 scale)
{
  this->scale = scale;
  calculated = false;
}