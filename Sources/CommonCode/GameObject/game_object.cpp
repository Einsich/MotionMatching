#include "game_object.h"
#include "CommonCode/Light/direction_light.h"
Transform &GameObject::get_transform()
{
  return transform;
}
void GameObject::render(const Camera& mainCam, const DirectionLight& light)
{

  light.bind_to_shader(shader);
  mainCam.set_to_shader(shader);
  material->bind_to_shader(shader);
  transform.set_to_shader(shader);

  mesh->render();

  material->unbind_to_shader(shader);
  light.unbind_to_shader(shader);
}
MaterialPtr GameObject::get_material()
{
  return material;
}
void GameObject::set_shader(const Shader & shader)
{
  this->shader = shader;
}
Shader& GameObject::get_shader()
{
  return shader;
}
MeshPtr GameObject::get_mesh()
{
  return mesh;
}

GameObjectPtr make_game_object(const Transform& transform, MeshPtr mesh_ptr, MaterialPtr material_ptr, const Shader& shader)
{
  return make_shared<GameObject>(transform, mesh_ptr, material_ptr, shader);
}

GameObjectPtr create_plane(const Transform &transform, bool create_uv)
{
  vector<vec3> vertices = {vec3(-1,0,-1), vec3(1,0,-1), vec3(1,0,1), vec3(-1,0,1)};
  vector<vec3> normals(4, vec3(0,1,0));
  vector<uint> indices = {0,1,3,1,2,3};
  MeshPtr currentMesh;
  MaterialPtr material = create_uv ? standart_textured_material(nullptr): standart_material();
  if (create_uv)
  {
    vector<vec2> uv =  {vec2(0,0), vec2(1,0), vec2(1,1),vec2(0,1)};
    static MeshPtr uvMesh = make_mesh(VertexArrayObject(indices, vertices, normals, uv));
    currentMesh = uvMesh;
  }
  else
  {
    static MeshPtr notUvMesh = make_mesh(VertexArrayObject(indices, vertices, normals));
    currentMesh = notUvMesh;
  }
  return make_game_object(transform, currentMesh, material, Shader());  
}
GameObjectPtr create_cube(const Transform &transform, bool create_uv)
{
  return nullptr;
}
GameObjectPtr create_sphere(const Transform &transform, bool create_uv)
{
  return nullptr;
}