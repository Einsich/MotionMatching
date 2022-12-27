#include <ecs/ecs.h>
#include "Animation/animation_player.h"
#include "Animation/animation_ik.h"
#include <render/debug_arrow.h>
#include "Animation/settings.h"
#include "Animation/third_person_controller.h"
#include <render/mesh.h>


SYSTEM(stage=act) animation_player_update(
  AnimationPlayer &animationPlayer,
  Settings &settings)
{
  float dt = Time::delta_time();

  if (animationPlayer.playerType == AnimationPlayerType::AnimationPlayer)
  {
    animationPlayer.index.update(dt, settings.lerpTime);
  }
  
  animationPlayer.currentCadr = animationPlayer.index.get_lerped_cadr();
  animationPlayer.rootDeltaTranslation = animationPlayer.currentCadr.rootLinearVelocity;
  animationPlayer.rootDeltaRotation = animationPlayer.currentCadr.rootAngularVelocity;
  animationPlayer.tree.set_cadr(animationPlayer.currentCadr);
  animationPlayer.tree.calculate_bone_transforms();
}

EVENT() init_animation_character(
  const ecs::OnEntityCreated &,
  AnimationPlayer &animationPlayer)
{
  if (animationPlayer.dataBase)
  {
    animationPlayer.dataBase.load();
    animationPlayer.dataBase->acceleration_structs(true);
    animationPlayer.index = AnimationLerpedIndex(animationPlayer.dataBase, animationPlayer.clip, animationPlayer.frame);
    if (animationPlayer.index)
      animationPlayer.currentCadr = AnimationCadr(animationPlayer.index.get_lerped_cadr());
    animationPlayer.tree = AnimationTree(&animationPlayer.dataBase->tree);
    animationPlayer.motionMatching = 
      MotionMatching(animationPlayer.dataBase, animationPlayer.index);
  }
  else
    return;
  AnimationTree &tree = animationPlayer.tree;
  tree.set_cadr(animationPlayer.currentCadr);
  tree.calculate_bone_transforms();
}

SYSTEM( stage=act) update_bone_remap(
  AnimationPlayer &animationPlayer,
  const Asset<Mesh> &mesh,
  bool &bone_remaped)
{
  if (!bone_remaped && mesh.loaded())
  {
    const AnimationTree &tree = animationPlayer.tree;
    animationPlayer.treeBoneToMesh.resize(tree.nodes.size());
    for (uint i = 0, n = tree.nodes.size(); i < n && mesh; i++)
    {
      auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
      animationPlayer.treeBoneToMesh[i] = 
        (it2 != mesh->bonesMap.end()) ? it2->second : -1;
    }
    bone_remaped = true;
  }
}
EVENT(tags=game) init_animation_material(
  const ecs::OnEntityCreated &,
  Asset<Material> &material)
{
  if (material && !material.is_copy())
    material = material.copy();
}