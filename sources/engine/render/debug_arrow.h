#pragma once
#include "3dmath.h"
#include "material.h"
#include "mesh.h"
#include "../transform.h"
#include <ecs/singleton.h>
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
  Mesh arrow;
  eastl::vector<Arrow> depthIgnore;
  eastl::vector<Arrow> depthNotIgnore;
  void add_triangle(vec3 a, vec3 b, vec3 c, std::vector<uint> &indices, std::vector<vec3> &vert, std::vector<vec3> &normal);
  void render_depth_case(UniformBuffer &instanceData, eastl::vector<Arrow> &arrows, bool ignoreDepth, bool wire_frame);
public:
  DebugArrow();
  void add_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore);

  void render(UniformBuffer &instanceData, bool wire_frame = false) ;
};
void draw_arrow(const mat4 &transform, const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_transform(const Transform &transform);
