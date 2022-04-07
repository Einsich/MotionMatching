#pragma once

#include <3dmath.h>

struct Frustum
{
  Frustum()=default;
  vec4 topFace;
  vec4 bottomFace;

  vec4 rightFace;
  vec4 leftFace;

  vec4 farFace;
  vec4 nearFace;
};
struct Camera;

Frustum create_frustum_from_camera(const Camera& cam, const Transform &transform);

bool isOnFrustum(const Frustum& camFrustum, vec3 world_pos, float world_radius);