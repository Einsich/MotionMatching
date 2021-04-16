#include <vector>
#include "debug_arrow.h"
#include "Mesh/vertex_array_object.h"
#include "Light/direction_light.h"
#include "Shader/shader.h"
#include "Camera/camera.h"

static DebugArrow *arrow_instance = nullptr;

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
void draw_transform(const Transform &transform)
{
  vec3 p = transform.get_position();
  mat3 rotation = transform.get_rotation();
  for (int i = 0; i < 3; i++)
  {
    vec3 n(0.f);
    n[i] = 1.f;
    draw_arrow(p, p + rotation * n * 0.3f, n, 0.03f, false);
  }
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

DebugArrow::DebugArrow(DebugArrow &&other)
{
  arrow_instance = this;
  arrowShader = other.arrowShader;
  arrowMaterial = other.arrowMaterial;
  arrow = other.arrow;
}

DebugArrow::DebugArrow(const DebugArrow &other)
{
  arrow_instance = this;
  arrowShader = other.arrowShader;
  arrowMaterial = other.arrowMaterial;
  arrow = other.arrow;
}
mat4 directionMatrix(vec3 from, vec3 to) {

  from = normalize(from);
  to = normalize(to);
  quat q(from, to);
  return toMat4(q);
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
void render_instancing(bool ignoreDepth, const Shader &shader, vector<mat4> &matrices, vector<vec3> &colors, const VertexArrayObject &arrow, bool wire_frame)
{
  assert(matrices.size() == colors.size());
  glDepthFunc(ignoreDepth ? GL_ALWAYS : GL_LESS);
  glDepthMask(ignoreDepth ? GL_FALSE : GL_TRUE);
  constexpr int ARROW_DRAWCALL_LIMIT = 150;
  for (int i = 0; i < (int)matrices.size(); i += ARROW_DRAWCALL_LIMIT)
  {
    int k = glm::min(ARROW_DRAWCALL_LIMIT, (int)matrices.size() - i);
    auto beginT = matrices.begin() + i;
    auto endT = matrices.begin() + i + k;
    shader.set_mat4x4("BoneTransforms", beginT, endT);
    auto beginC = colors.begin() + i;
    auto endC = colors.begin() + i + k;
    shader.set_vec3("Colors", beginC, endC);
    arrow.render_instances(k, wire_frame);
  }
  matrices.clear();
  colors.clear();
}
void DebugArrow::render(const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame)
{
  arrowShader.use();
  light.bind_to_shader(arrowShader);
  set_camera_to_shader(arrowShader, view_projection, camera_position);
  arrowMaterial->bind_to_shader(arrowShader);
  glDisable(GL_CULL_FACE);

  render_instancing(true, arrowShader, depthIgnore.arrowTransforms, depthIgnore.arrowColors, arrow, wire_frame);
  render_instancing(false, arrowShader, depthNotIgnore.arrowTransforms, depthNotIgnore.arrowColors, arrow, wire_frame);

  arrowMaterial->unbind_to_shader(arrowShader);
  light.unbind_to_shader(arrowShader);
  glEnable(GL_CULL_FACE);
}
