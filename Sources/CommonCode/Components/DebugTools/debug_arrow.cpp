#include <vector>
#include "debug_arrow.h"
#include "CommonCode/Mesh/vertex_array_object.h"
#include "CommonCode/Light/direction_light.h"
#include "CommonCode/Shader/shader.h"

static DebugArrow* arrow_instance;

void draw_arrow(const mat4 &transform, const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore)
{
  if (arrow_instance)
    arrow_instance->add_arrow(transform * vec4(from, 1), transform * vec4(to, 1), color, size, depth_ignore);
}
void draw_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore)
{
  if (arrow_instance)
    arrow_instance->add_arrow(from, to, color, size, depth_ignore);

}
void DebugArrow::add_triangle(vec3 a, vec3 b, vec3 c, vector<uint> &indices, vector<vec3> &vert, vector<vec3> &normal)
{
  uint k = vert.size();
  vec3 n = normalize(cross(b - a, c - a));
  indices.push_back(k); indices.push_back(k + 1); indices.push_back(k + 2);
  vert.push_back(a); vert.push_back(b); vert.push_back(c);
  normal.push_back(n); normal.push_back(n); normal.push_back(n);
  
}
DebugArrow::DebugArrow()
{
  if (arrow_instance)
  {
    debug_error("Debug arrow instance already exist!");
    return;
  }
  else
  {
    arrow_instance = this;
  }
  arrowShader = get_shader("bones");
  arrowMaterial = standart_material();
  vector<uint> indices;
  vector<vec3> vert;
  vector<vec3> normal;
  vec3 c = vec3(0, 1, 0);
  const int N = 4;
  vec3 p[N];
  for (int i = 0; i < N; i++)
  {
    float a1 = ((float)(i) / N) * 2 * PI;
    float a2 = ((float)(i + 1) / N) * 2 * PI;
    vec3 p1 = p[i] = vec3(cos(a1), 0, sin(a1));
    vec3 p2 = vec3(cos(a2), 0, sin(a2));
    add_triangle(p2, p1, c, indices, vert, normal);
  }

  add_triangle(p[0], p[1], p[2], indices, vert, normal);
  add_triangle(p[0], p[2], p[3], indices, vert, normal);
  arrow = VertexArrayObject(indices, vert, normal);
  
  
}
mat4 directionMatrix(vec3 from, vec3 to) {

  from = normalize(from);
  to = normalize(to);
  vec3 V = (cross(from, to));
  if (length2(V) < 1e-6)
  {
    mat4 m(1.f);
    for (uint i = 0; i < 3; i++)
      m[i][i] = abs(from[i]) < 1e-4 ? 1 : to[i]/ from[i]; 
    return m;
  }
  V = normalize(V);
  float phi = -acos(dot(from, to));

  float rcos = cos(phi);
  float rsin = sin(phi);

  mat4 M;

  M[0].x =        rcos + V.x * V.x * (1.0 - rcos);
  M[1].x =  V.z * rsin + V.y * V.x * (1.0 - rcos);
  M[2].x = -V.y * rsin + V.z * V.x * (1.0 - rcos);
  M[3].x = 0.0;

  M[0].y = -V.z * rsin + V.x * V.y * (1.0 - rcos);
  M[1].y =        rcos + V.y * V.y * (1.0 - rcos);
  M[2].y = -V.x * rsin + V.z * V.y * (1.0 - rcos);
  M[3].y = 0.0;

  M[0].z =  V.y * rsin + V.x * V.z * (1.0 - rcos);
  M[1].z = -V.x * rsin + V.y * V.z * (1.0 - rcos);
  M[2].z =        rcos + V.z * V.z * (1.0 - rcos);
  M[3].z = 0.0;

  M[0].w = 0.0;
  M[1].w = 0.0;
  M[2].w = 0.0;
  M[3].w = 1.0;

    return M;
}
void DebugArrow::add_arrow(const vec3 &from, const vec3 &to, vec3 color, float size, bool depth_ignore)
{
  vec3 d = to - from;
  mat4 t = translate(mat4(1.f), from);
  float len = length(d);
  mat4 s = scale(mat4(1.f), vec3(size, len, size));

  mat4 r = directionMatrix(vec3(0, 1, 0), d);
  if (depth_ignore)
  {
    depthIgnore.arrowTransforms.push_back(t * r * s);
    depthIgnore.arrowColors.push_back(color);
  }
  else
  {
    depthNotIgnore.arrowTransforms.push_back(t * r * s);
    depthNotIgnore.arrowColors.push_back(color);
  }  
}

void DebugArrow::render(const Camera& mainCam, const DirectionLight& light, bool wire_frame)
{
  arrowShader.use();
  light.bind_to_shader(arrowShader);
  mainCam.set_to_shader(arrowShader);
  arrowMaterial->bind_to_shader(arrowShader);
  {
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);
    arrowShader.set_mat4x4("BoneTransforms", depthIgnore.arrowTransforms);
    arrowShader.set_vec3("Colors", depthIgnore.arrowColors);
    arrow.render_instances(depthIgnore.arrowTransforms.size());
    depthIgnore.arrowTransforms.clear();
    depthIgnore.arrowColors.clear();
  }
  {
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    arrowShader.set_mat4x4("BoneTransforms", depthNotIgnore.arrowTransforms);
    arrowShader.set_vec3("Colors", depthNotIgnore.arrowColors);
    arrow.render_instances(depthNotIgnore.arrowTransforms.size());
    depthNotIgnore.arrowTransforms.clear();
    depthNotIgnore.arrowColors.clear();
  }
  arrowMaterial->unbind_to_shader(arrowShader);
  light.unbind_to_shader(arrowShader);
  
}
