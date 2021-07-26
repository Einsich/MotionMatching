#include "mesh_render.h"
#include "Engine/camera.h"
MeshRender::MeshRender():shader(::get_shader("standart_normal"))
{

}
MeshRender::MeshRender(Asset<Mesh> mesh_ptr, Asset<Material> materail_ptr, const Shader& shader):
  mesh(mesh_ptr), material(materail_ptr), shader(shader)
{
  
}

void MeshRender::render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame) const 
{
  if (!material || !mesh)
    return;
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