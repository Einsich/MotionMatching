#include "bone_render.h"
#include "CommonCode/Material/material.h"

void add_triangle(vec3 a, vec3 b, vec3 c, vector<uint> &indices, vector<vec3> &vert, vector<vec3> &normal)
{
  uint k = vert.size();
  vec3 n = normalize(cross(b - a, c - a));
  indices.push_back(k); indices.push_back(k + 1); indices.push_back(k + 2);
  vert.push_back(a); vert.push_back(b); vert.push_back(c);
  normal.push_back(n); normal.push_back(n); normal.push_back(n);
  
}
BoneRender::BoneRender()
{
  boneShader = get_shader("bones");
  boneMaterial = standart_material();
  boneMaterial->set_property(Property("Diffuse", vec3(0,0.8f,0)));
  boneMaterial->set_property(Property("Ambient", vec3(0,0.2f,0)));
  vector<uint> indices;
  vector<vec3> vert;
  vector<vec3> normal;
  vec3 c = vec3(0, 1, 0);
  const int N = 4;
  for (int i = 0; i < N; i++)
  {
    float a1 = ((float)(i) / N) * 2 * PI;
    float a2 = ((float)(i + 1) / N) * 2 * PI;
    vec3 p1 = vec3(cos(a1), 0, sin(a1));
    vec3 p2 = vec3(cos(a2), 0, sin(a2));
    add_triangle(p1, p2, c, indices, vert, normal);
  }
  arrow = VertexArrayObject(indices, vert, normal);
}
mat4 directionMatrix(vec3 from, vec3 to) {

  from = normalize(from);
  to = normalize(to);
  vec3 V = normalize(cross(from, to));
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
void BoneRender::calculate_transforms(const mat4& transform, const AnimationTree &tree)
{
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    const AnimationNode &node = tree.nodes[i];
    if (node.parent >= 0)
    {
      boneTransforms[i] = boneTransforms[node.parent] * node.animationTransform;
    }
    else
    {
      boneTransforms[i] = transform * node.animationTransform;
    }    
    boneOffsets[i] = vec3(boneTransforms[i][3]);
  }
  mat4 flip = mat4(1.f);
  flip[0] = vec4(0,1,0,0);
  flip[1] = vec4(1,0,0,0);
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    vec3 p = tree.nodes[i].parent >= 0 ? boneOffsets[tree.nodes[i].parent] : vec3(0.f);
    vec3 d = boneOffsets[i] - p;
    mat4 t = translate(mat4(1.f), p);
    float len = length(d);
    float width = std::min(1.5f, len * 0.13f);
    mat4 s = scale(mat4(1.f), vec3(width, len, width));

    mat4 r = directionMatrix(vec3(0, 1, 0), d);
    boneTransforms[i] = t * r * s;
  }
}
void BoneRender::render(const mat4& transform, const AnimationTree &tree, const Camera& mainCam, const DirectionLight& light)
{
  uint n = tree.nodes.size();
  if (boneTransforms.size() != n)
  {
    boneTransforms.resize(n);
    boneOffsets.resize(n);
  }
  calculate_transforms(transform, tree);
  boneShader.use();
  light.bind_to_shader(boneShader);
  mainCam.set_to_shader(boneShader);
  boneMaterial->bind_to_shader(boneShader);
  boneShader.set_mat4x4("BoneTransforms", boneTransforms);
  glDepthFunc(GL_ALWAYS);
  arrow.render_instances(n);
  glDepthFunc(GL_LESS);
  boneMaterial->unbind_to_shader(boneShader);
  light.unbind_to_shader(boneShader);
}