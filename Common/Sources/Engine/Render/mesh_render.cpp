#include "mesh_render.h"
#include "Engine/camera.h"
MeshRender::MeshRender()
{

}
MeshRender::MeshRender(Asset<Mesh> mesh, Asset<Material> material):
  mesh(mesh), material(material)
{
  
}

void MeshRender::render(const Transform &transform, bool wire_frame) const 
{
  if (!material || !mesh)
    return;
  const Shader &shader = material->get_shader();
  shader.use();
  material->bind_to_shader();
  transform.set_to_shader(shader);

  mesh->render(wire_frame);

  material->unbind_to_shader();
}
const Asset<Material>& MeshRender::get_material() const
{
  return material;
}
Asset<Material>& MeshRender::get_material()
{
  return material;
}