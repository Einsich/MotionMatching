#include "transform2d.h"
#include <ecs/registration.h>

ECS_REGISTER_TYPE(Transform2D, "Transform2D", ecs::PODType)


mat3 Transform2D::get_matrix() const
{
  float rotcos = cos(rotation), rotsin = sin(rotation);
  return mat3{
    {rotcos * scale.x, -rotsin * scale.y, 0},
    {rotsin * scale.x, rotcos * scale.y, 0},
    {position.x, position.y, 1}};
}