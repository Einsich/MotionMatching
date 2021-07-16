#include "mesh_render.h"
#include "Engine/camera.h"

MeshRender::MeshRender(MeshPtr mesh_ptr, Asset<Material> materail_ptr, const Shader& shader):
  mesh(mesh_ptr), material(materail_ptr), shader(shader)
{
  
}

void MeshRender::render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame) const 
{
  shader.use();
  light.bind_to_shader(shader);
  set_camera_to_shader(shader, view_projection, camera_position);
  material->bind_to_shader(shader);
  transform.set_to_shader(shader);

  mesh->render(wire_frame);

  material->unbind_to_shader(shader);
  light.unbind_to_shader(shader);
}
Asset<Material> MeshRender::get_material() const
{
  return material;
}
const Shader& MeshRender::get_shader() const
{
  return shader;
}
Shader& MeshRender::get_shader()
{
  return shader;
}
MeshRender create_plane(bool create_uv)
{
  return create_plane(create_uv ? standart_textured_material(Asset<Texture2D>()): standart_material(), create_uv);
}
MeshRender create_plane(Asset<Material> material, bool create_uv)
{
  static MeshPtr uvMesh = nullptr;
  static MeshPtr notUvMesh = nullptr;
  if (!uvMesh || !notUvMesh)
  {
    vector<vec3> vertices = {vec3(-1,0,-1), vec3(1,0,-1), vec3(1,0,1), vec3(-1,0,1)};
    vector<vec3> normals(4, vec3(0,1,0));
    vector<uint> indices = {0,1,2,0,2,3};
    vector<vec2> uv =  {vec2(0,0), vec2(1,0), vec2(1,1),vec2(0,1)};
    uvMesh = make_mesh(VertexArrayObject(indices, vertices, normals, uv));
    notUvMesh = make_mesh(VertexArrayObject(indices, vertices, normals));
  }

  MeshPtr mesh = create_uv ? uvMesh : notUvMesh;
  return MeshRender(mesh, material, create_uv ? get_shader("standart_normal_uv") : get_shader("standart_normal"));  
}
MeshRender create_cube(bool create_uv)
{
  return create_cube(create_uv ? standart_textured_material(Asset<Texture2D>()): standart_material(), create_uv);
}
MeshRender create_cube(Asset<Material> material, bool create_uv)
{
  static MeshPtr uvMesh = nullptr;
  static MeshPtr notUvMesh = nullptr;
  if (!uvMesh || !notUvMesh)
  {
    vector<uint> indices;
    vector<vec3> vertices, normals;
    vector<vec2> uv;
    for (int face = 0; face < 3; face++)
    {
      for (int d = -1; d <= 1; d += 2)
      {
        vec3 norm = vec3();
        norm[face] = d;
        int ind = vertices.size();
        float a = -1, b = -1, ta, tb;
        for (int i = 0; i < 4; i++)
        {
          vec3 v;
          v[face] = d;
          v[(face + 1) % 3] = a;
          v[(face + 2) % 3] = b;

          vec2 u;
          if (face != 1)
          {
            u.x = (v.x + v.z - d + 1.f) * 0.5f;
            if ((d < 0) ^ (face == 0))
              u.x = 1.f - u.x;
            u.y = 1.f - (v.y + 1.f) * 0.5f;
          }
          else
          {
            u = (vec2(a,b) + vec2(1.f))*0.5f;
          }

          ta = -b * d;
          tb = a * d;
          a = ta;
          b = tb;
          vertices.push_back(v);
          normals.push_back(norm);
          uv.push_back(u);
        }
        indices.push_back(ind); indices.push_back(ind + 2);indices.push_back(ind + 1);
        indices.push_back(ind); indices.push_back(ind + 3); indices.push_back(ind + 2);
      }
    }
    uvMesh = make_mesh(VertexArrayObject(indices, vertices, normals, uv));
    notUvMesh = make_mesh(VertexArrayObject(indices, vertices, normals));
  }
  MeshPtr mesh = create_uv ? uvMesh : notUvMesh;
  return MeshRender(mesh, material, create_uv ? get_shader("standart_normal_uv") : get_shader("standart_normal"));  
}

MeshRender create_sphere(int detailed, bool smooth, bool create_uv)
{
  return create_sphere(create_uv ? standart_textured_material(Asset<Texture2D>()): standart_material(), detailed, smooth, create_uv);
}
MeshRender create_sphere(Asset<Material> material, int detailed, bool smooth, bool create_uv)
{
  detailed = glm::clamp(detailed, 1, 20);
  int t = (int)smooth;
  t = t * 2 + (int)create_uv;
  t = t + detailed * 4;
  static map<int, MeshPtr> spheres;
  MeshPtr mesh;
  if (spheres[t] != nullptr)
    mesh = spheres[t];
  else
  {
    int n = detailed + 2;
    int m = detailed + 2;

    vector<uint> indices;
    vector<vec3> vertices, normals;
    vector<vec2> uv;
    for (int i = 0; i <= n; i++)
      for (int j = 0; j <= m; j++)
      {
        float u = (1.f * i / m);
        float v = (1.f * j / n);
        float b = u * PI * 2;
        float a = (v - 0.5f) * PI;
        float x = cos(a) * cos(b);
        float z = cos(a) * sin(b);
        float y = sin(a);
        vec3 p = vec3(x, y, z);
        vertices.push_back(p);
        normals.push_back(p);
        uv.push_back(vec2(u, 1.f - v));
      }
    
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
      {
        int a = i * (n + 1) + j;
        int b = a + 1;
        int c = a + n + 1;
        int d = c + 1;
        indices.push_back(a); indices.push_back(d); indices.push_back(b);
        indices.push_back(a); indices.push_back(c); indices.push_back(d);
      }
    mesh = spheres[t] =  make_mesh(VertexArrayObject(indices, vertices, normals, uv));
  }
  return MeshRender(mesh, material, create_uv ? get_shader("standart_normal_uv") : get_shader("standart_normal"));  
}
