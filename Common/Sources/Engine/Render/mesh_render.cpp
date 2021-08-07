#include "mesh_render.h"
#include "Engine/camera.h"
MeshRender::MeshRender()
{

}
MeshRender::MeshRender(Asset<Mesh> mesh, Asset<Material> material):
  mesh(mesh), material(material)
{
  
}

void MeshRender::render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, bool wire_frame) const 
{
  if (!material || !mesh)
    return;
  const Shader &shader = material->get_shader();
  shader.use();
  light.bind_to_shader(shader);
  set_camera_to_shader(shader, view_projection, camera_position);
  material->bind_to_shader();
  transform.set_to_shader(shader);

  mesh->render(wire_frame);

  material->unbind_to_shader();
  light.unbind_to_shader(shader);
}
const Asset<Material>& MeshRender::get_material() const
{
  return material;
}
Asset<Material>& MeshRender::get_material()
{
  return material;
}