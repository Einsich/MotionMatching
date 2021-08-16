#include <ecs/ecs.h>
#include "Animation/animation_player.h"
#include "Animation/animation_ik.h"
#include <Engine/Render/debug_arrow.h>
#include <Engine/Physics/physics.h>
#include <Engine/Profiler/profiler.h>
#include <Engine/camera.h>
#include "Animation/settings.h"
#include "Animation/third_person_controller.h"

bool raycast_in_point(vec3 point, float &dh, vec3 &normal)
{
  float h = 10;
  Ray ray(point + vec3(0,h,0), vec3(0,-1,0), 100);
  Collision collision = ray_cast(ray);
  if (collision.collider)
  {
    dh =  collision.distance - h;
    normal = collision.collisionNormal;
  }
  return collision.collider != nullptr;
}
bool on_ground(float h)
{
  return -0.5f <= h && h <= 0.00;
}
template<typename C>
void detect_motion_matching_lod(C);

SYSTEM(ecs::SystemOrder::LOGIC) animation_player_update(
  Transform &transform,
  AnimationPlayer &animationPlayer,
  Asset<Material> &material,
  ThirdPersonController *thirdPersonController,
  int *mmIndex,
  int *mmOptimisationIndex,
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
    for (; j < 3; j++)
      if (dist < OptimisationSettings.lodDistances[j])
      { 
        lodColor[j] = 10;
        break;
      }
    if (j == 0)
      lodColor = vec3(1);
    if (j == 3)
      lodColor = vec3(10);
    animationPlayer.motionMatching.lod = j;
    
    material->set_property("material.Diffuse", lodColor);
    //static int i = 0;
    //ProfilerLabel motion_matching("motion_matching" + to_string(i));
    //i = (i + 1) % (settings.testCount + 1);
    animationPlayer.motionMatching.update(dt, animationPlayer.inputGoal, mmsettings, OptimisationSettings, thirdPersonController != nullptr, settings);
    animationPlayer.index = animationPlayer.motionMatching.get_index();
  }
  if (animationPlayer.playerType == AnimationPlayerType::AnimationPlayer)
  {
    animationPlayer.index.update(dt, settings.lerpTime);
  }
  float ticks = animationPlayer.index.ticks_per_second();
  
  animationPlayer.currentCadr = animationPlayer.index.get_lerped_cadr();
  animationPlayer.rootDeltaTranslation = animationPlayer.currentCadr.rootTranslationDelta * ticks;
  animationPlayer.rootDeltaRotation = animationPlayer.currentCadr.rootRotationDelta * ticks;
  
  if (false)//get_bool_config("UseIK"))
  {
    mat4 t = transform.get_transform();
    vec3 hips = animationPlayer.index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::Hips];
    vec3 leftToe = animationPlayer.index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::LeftToeBase];
    vec3 rightToe = animationPlayer.index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::RightToeBase];
    
   animationPlayer.onGround = animationPlayer.index.current_index().get_clip().onGround[animationPlayer.index.current_index().get_cadr_index()];
    float h = 0;
    vec3 normal, leftToeNormal, rightToeNormal;
    normal = leftToeNormal = rightToeNormal = vec3(0, 1, 0);
    constexpr float ground_value = 0.04f;
    if (!animationPlayer.ikFoot[0].onGround && raycast_in_point(t * vec4(leftToe, 1), h, leftToeNormal))
    { 
      h -= ground_value;
      if (on_ground(h))
      {
        animationPlayer.onGround |= 1;
      }
      leftToe.y -= h;
    }
    if (!animationPlayer.ikFoot[1].onGround && raycast_in_point(t * vec4(rightToe, 1), h, rightToeNormal))
    { 
      h -= ground_value;
      if (on_ground(h))
      {
        animationPlayer.onGround |= 2;
      }
      rightToe.y -= h;
    }
    float dh = 0;
    if (raycast_in_point(t * vec4(hips, 1), dh, normal))
    {
      dh -= hips.y;
      dh *= 15;
      animationPlayer.rootDeltaTranslation.y += dh;
    }
    
    mat4 inv_t = inverse(t);
    leftToeNormal = inv_t * vec4(leftToeNormal, 0);
    rightToeNormal = inv_t * vec4(rightToeNormal, 0);
    //set_data_to_IK(t, 0, leftFoot, leftToe, leftToeNormal, "LeftFoot", "LeftToeBase");
    //set_data_to_IK(t, 1, rightFoot, rightToe, rightToeNormal, "RightFoot", "RightToeBase");
    
    int hipsNode = animationPlayer.tree.get_child("Hips");
    for (int i = 0; i < 2; i++)
    {
      if (animationPlayer.ikFoot[i].onGround)
      {
        vec3 footPosition = inv_t * vec4(animationPlayer.ikFoot[i].footPosition, 1);
        vec3 toePosition = inv_t * vec4(animationPlayer.ikFoot[i].toePosition, 1);
        process_IK(animationPlayer.tree, animationPlayer.currentCadr, t, footPosition, animationPlayer.ikFoot[i].normal, animationPlayer.ikFoot[i].footNode, hipsNode);
        process_IK(animationPlayer.tree, animationPlayer.currentCadr, t, toePosition, vec3(0,1,0), animationPlayer.ikFoot[i].toeNode, hipsNode);
        vec3 norm = t * vec4(animationPlayer.ikFoot[i].normal, 0);
        draw_arrow(animationPlayer.ikFoot[i].toePosition, animationPlayer.ikFoot[i].toePosition + norm * 0.3f, vec3(10,0,0), 0.02f, false);
      }
    }
  }
  animationPlayer.tree.set_cadr(animationPlayer.currentCadr);
  animationPlayer.tree.calculate_bone_transforms();
}

EVENT(ecs::SystemTag::GameEditor) init_animation_character(
  const ecs::OnEntityCreated &,
  AnimationPlayer &animationPlayer)
{
  animationPlayer.tree.set_cadr(animationPlayer.currentCadr);
  animationPlayer.tree.calculate_bone_transforms();
}
EVENT(ecs::SystemTag::Game) init_animation_material(
  const ecs::OnEntityCreated &,
  Asset<Material> &material)
{
  material = material.copy();
}