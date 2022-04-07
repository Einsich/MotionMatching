#include "camera.h"
#include "frustum.h"

static vec4 plan(vec3 point, vec3 normal)
{
  normal = normalize(normal);
  return vec4(normal, dot(normal, point));
}

Frustum create_frustum_from_camera(const Camera& cam, const Transform &transform)
{
  Frustum frustum;
  vec3 dir[3] = {-transform.get_forward(), -transform.get_right(), transform.get_up()};
  vec3 pos = transform.get_position();
  const float halfVSide = cam.zFar * tanf(cam.fieldOfView * DegToRad * .5f);

  const float halfHSide = halfVSide * cam.aspectRatio;
  const glm::vec3 frontMultFar = cam.zFar * dir[0];

  frustum.nearFace = plan(pos + cam.zNear * dir[0], dir[0]);
  frustum.farFace = plan(pos + frontMultFar, -dir[0]);
  frustum.rightFace = plan(pos, glm::cross(dir[2], frontMultFar + dir[1] * halfHSide));
  frustum.leftFace = plan(pos, glm::cross(frontMultFar - dir[1] * halfHSide, dir[2]));
  frustum.topFace = plan(pos, glm::cross(dir[1], frontMultFar - dir[2] * halfVSide));
  frustum.bottomFace = plan(pos, glm::cross(frontMultFar + dir[2] * halfVSide, dir[1]));

  return frustum;
}

static bool isOnOrForwardPlan(const vec4& plan, const vec3 &s, float radius)
{
  vec3 p = plan;
  return glm::dot(p, s) - plan.w + radius > 0;
}

bool isOnFrustum(const Frustum& camFrustum, vec3 world_pos, float world_radius)
{
  return 
    isOnOrForwardPlan(camFrustum.nearFace, world_pos, world_radius) &&
    isOnOrForwardPlan(camFrustum.leftFace, world_pos, world_radius) &&
    isOnOrForwardPlan(camFrustum.rightFace, world_pos, world_radius) &&
    isOnOrForwardPlan(camFrustum.topFace, world_pos, world_radius) &&
    isOnOrForwardPlan(camFrustum.bottomFace, world_pos, world_radius) &&
    isOnOrForwardPlan(camFrustum.farFace, world_pos, world_radius);

};