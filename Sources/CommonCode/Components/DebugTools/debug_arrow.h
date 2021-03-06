#pragma once
#include "CommonCode/math.h"
#include "CommonCode/component.h"
#include "CommonCode/Material/material.h"
#include "CommonCode/Camera/camera.h"
#include "CommonCode/Light/direction_light.h"
#include "CommonCode/Mesh/vertex_array_object.h"

class DebugArrow : public Component, public IRenderable
{
private:
  struct ArrowSet
  {
    vector<mat4> arrowTransforms;
    vector<vec3> arrowColors;
  };
  Shader arrowShader;
  MaterialPtr arrowMaterial;
  VertexArrayObject arrow;
  ArrowSet depthIgnore;
  ArrowSet depthNotIgnore;
  void add_triangle(vec3 a, vec3 b, vec3 c, vector<uint> &indices, vector<vec3> &vert, vector<vec3> &normal);
public:
  DebugArrow();
  void add_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore);

  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;
};
void draw_arrow(const mat4 &transform, const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);
void draw_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore = true);