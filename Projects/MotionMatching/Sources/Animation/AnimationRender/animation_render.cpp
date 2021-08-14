#include "animation_render.h"
#include <Engine/camera.h>

static vector<mat4> curTransform;
AnimationRender::AnimationRender(Asset<Mesh> mesh, Asset<Material> material):
    mesh(mesh), material(material)
  {}

void AnimationRender::process(const Transform &transform, const AnimationTree &tree)
{
  if (!mesh)
    return;
  curTransform.resize(mesh->bonesMap.size());
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
    if (it2 != mesh->bonesMap.end())
    {
      curTransform[it2->second] = tree.get_bone_transform(i);
    }
  }
  material->set_property("Bones[0]", curTransform);
  material->set_property("Model", transform.get_transform());
}
void AnimationRender::render(bool wire_frame) const
{
  mesh->render(wire_frame);
}
const Asset<Material>& AnimationRender::get_material() const
{
  return material;
}
Asset<Material>& AnimationRender::get_material()
{
  return material;
}
const Asset<Mesh>& AnimationRender::get_mesh() const
{
  return mesh;
}
Asset<Mesh>& AnimationRender::get_mesh()
{
  return mesh;
}