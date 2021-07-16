#pragma once
#include "3dmath.h"
#include "material.h"
#include "direction_light.h"
#include "vertex_array_object.h"
#include "../transform.h"
#include "ecs/singleton.h"
class DebugArrow : ecs::Singleton
{
private:
  struct ArrowSet
  {
    vector<mat4> arrowTransforms;
    vector<vec3> arrowColors;
  };
  Shader arrowShader;
  Asset<Material> arrowMaterial;
  VertexArrayObject arrow;
  ArrowSet depthIgnore;
  ArrowSet depthNotIgnore;
  void add_triangle(vec3 a, vec3 b, vec3 c, vector<uint> &indices, vector<vec3> &vert, vector<vec3> &normal);
public:
  DebugArrow();
  void add_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore);

  void render(const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame = false) ;
};
void draw_arrow(const mat4 &transform, const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_transform(const Transform &transform);
