#include "animation_render.h"

AnimationRender::AnimationRender(GameObjectPtr gameObject) :
  curTransform(gameObject->get_mesh()->bonesMap.size()), gameObject(gameObject)
  {}

void AnimationRender::render(const Camera& mainCam, const DirectionLight& light, const AnimationTree &tree) 
{
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    auto it2 = gameObject->get_mesh()->bonesMap.find(tree.nodes[i].get_name());
    if (it2 != gameObject->get_mesh()->bonesMap.end())
    {
      curTransform[it2->second] = tree.nodes[i].get_bone_transform();
    }
  }
  gameObject->get_shader().use();
  gameObject->get_shader().set_mat4x4("Bones", curTransform, false);
  gameObject->render(mainCam, light);
  if (renderBones)
    boneRender.render(gameObject->get_transform().get_transform(), tree, mainCam, light);
}