#include <ecs.h>
#include "Animation/animation_player.h"
#include "Animation/animation_ik.h"
#include <render/debug_arrow.h>
#include <profiler/profiler.h>
#include <camera.h>
#include "Animation/settings.h"
#include "Animation/third_person_controller.h"
#include <render/mesh.h>

template<typename C>
void detect_motion_matching_lod(C);

SYSTEM(stage=act) animation_player_update(
  Transform &transform,
  AnimationPlayer &animationPlayer,
  Asset<Material> &material,
  int *mmIndex,
  int *mmOptimisationIndex,
  bool updateMMStatistic,
  int motionMatchingSolver,
  Settings &settings,
  SettingsContainer &settingsContainer,
  const MainCamera &mainCamera)
{
  float dt = Time::delta_time();
  
  if (animationPlayer.playerType ==  AnimationPlayerType::MotionMatching)
  {
    const MotionMatchingSettings &mmsettings = settingsContainer.motionMatchingSettings[mmIndex ? *mmIndex : 0].second;
    const MotionMatchingOptimisationSettings &OptimisationSettings = 
      settingsContainer.motionMatchingOptimisationSettings[mmOptimisationIndex ? *mmOptimisationIndex : 0].second;
    float dist = length(mainCamera.position - transform.get_position());

    int j = 0;
    vec3 lodColor(0.f);
    vec3 lodColors[] = {vec3(0), vec3(0,1,0), vec3(0,0,1), vec3(1,0,0)};
    for (; j < 3; j++)
      if (dist < OptimisationSettings.lodDistances[j])
        break;
    lodColor = lodColors[j];
    animationPlayer.motionMatching.lod = j;
    
    material->set_property("material.AdditionalColor", lodColor);
    //static int i = 0;
    //ProfilerLabel motion_matching("motion_matching" + to_string(i));
    //i = (i + 1) % (settings.testCount + 1);
    animationPlayer.motionMatching.update(dt, (MotionMatchingSolverType)motionMatchingSolver, animationPlayer.inputGoal,
        mmsettings, OptimisationSettings, updateMMStatistic, settings);
    animationPlayer.index = animationPlayer.motionMatching.get_index();
  }
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

EVENT(scene=game, editor) init_animation_character(
  const ecs::OnEntityCreated &,
  Asset<Mesh> &mesh,
  const vector<Asset<Mesh>> *lods_meshes,
  AnimationPlayer &animationPlayer)
{
  if (animationPlayer.dataBase)
  {
    animationPlayer.dataBase.load();
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
  if (lods_meshes && !lods_meshes->empty())
    mesh = (*lods_meshes)[0];
  animationPlayer.treeBoneToMesh.resize(tree.nodes.size());
  for (uint i = 0, n = tree.nodes.size(); i < n && mesh; i++)
  {
    auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
    animationPlayer.treeBoneToMesh[i] = 
      (it2 != mesh->bonesMap.end()) ? it2->second : -1;
  }
}
EVENT(scene=game) init_animation_material(
  const ecs::OnEntityCreated &,
  Asset<Material> &material)
{
  if (material && !material.is_copy())
    material = material.copy();
}