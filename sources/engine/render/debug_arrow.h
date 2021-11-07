#pragma once
#include "3dmath.h"
#include "material.h"
#include "vertex_array_object.h"
#include "../transform.h"
#include "singleton.h"
#include "global_uniform.h"
class DebugArrow : ecs::Singleton
{
private:
  struct Arrow
  {
    mat4 transform;
    vec4 color;
  };
  Asset<Material> arrowMaterial;
  VertexArrayObject arrow;
  vector<Arrow> depthIgnore;
  vector<Arrow> depthNotIgnore;
  void add_triangle(vec3 a, vec3 b, vec3 c, vector<uint> &indices, vector<vec3> &vert, vector<vec3> &normal);
  void render_depth_case(UniformBuffer &instanceData, vector<Arrow> &arrows, bool ignoreDepth, bool wire_frame);
public:
  DebugArrow();
  void add_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore);

  void render(UniformBuffer &instanceData, bool wire_frame = false) ;
};
void draw_arrow(const mat4 &transform, const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_transform(const Transform &transform);
