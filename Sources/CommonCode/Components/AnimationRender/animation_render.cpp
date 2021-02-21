#include "animation_render.h"
#include "CommonCode/Animation/animation_player.h"
#include "CommonCode/GameObject/game_object.h"

static vector<mat4> curTransform;
AnimationRender::AnimationRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader, bool renderBones):
    mesh(mesh_ptr), material(materail_ptr), shader(shader), renderBones(renderBones)
  {}

void AnimationRender::render(const Camera& mainCam, const DirectionLight& light, bool wire_frame)
{
  AnimationPlayer *player = gameObject->get_component<AnimationPlayer>();
  Transform *transform = gameObject->get_component<Transform>();
  if (player && transform)
    render(*transform, mainCam, light, player->get_tree(), wire_frame);
}
void AnimationRender::render(const Transform &transform, const Camera& mainCam, const DirectionLight& light, const AnimationTree &tree, bool wire_frame)
{
  curTransform.resize(mesh->bonesMap.size());
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
    if (it2 != mesh->bonesMap.end())
    {
      curTransform[it2->second] = tree.nodes[i].get_bone_transform();
    }
  }
  shader.use();
  shader.set_mat4x4("Bones", curTransform, false);
  light.bind_to_shader(shader);
  mainCam.set_to_shader(shader);
  material->bind_to_shader(shader);
  transform.set_to_shader(shader);

  mesh->render(wire_frame);

  material->unbind_to_shader(shader);
  light.unbind_to_shader(shader);
  if (renderBones)
    boneRender.render(transform.get_transform(), tree, mainCam, light);
}
MaterialPtr AnimationRender::get_material() const
{
  return material;
}