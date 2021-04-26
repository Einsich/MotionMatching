#include "animation_render.h"
#include "Engine/camera.h"

static vector<mat4> curTransform;
AnimationRender::AnimationRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader):
    mesh(mesh_ptr), material(materail_ptr), shader(shader)
  {}

void AnimationRender::render(const Transform &transform, const mat4 view_projection, const vec3 &camera_position, const DirectionLight& light, const AnimationTree &tree, bool wire_frame) const
{
  
  curTransform.resize(mesh->bonesMap.size());
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
    if (it2 != mesh->bonesMap.end())
    {
      curTransform[it2->second] = tree.get_bone_transform(i);
    }
  }
  shader.use();
  shader.set_mat4x4("Bones", curTransform, false);
  light.bind_to_shader(shader);
  set_camera_to_shader(shader, view_projection, camera_position);
  material->bind_to_shader(shader);
  transform.set_to_shader(shader);

  mesh->render(wire_frame);

  material->unbind_to_shader(shader);
  light.unbind_to_shader(shader);
}
MaterialPtr AnimationRender::get_material() const
{
  return material;
}