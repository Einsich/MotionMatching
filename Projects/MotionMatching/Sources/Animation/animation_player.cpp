#include "animation_player.h"
#include "Engine/time.h"
#include "animation_ik.h"
#include "Engine/Render/debug_arrow.h"
#include "Engine/Physics/physics.h"
#include "config.h"
AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, string first_anim, AnimationPlayerType playerType):
playerType(playerType), speed(1.f), 
stateMachine(playerType ==  AnimationPlayerType::StateMachine ? dataBase : nullptr), 
motionMatching(playerType ==  AnimationPlayerType::MotionMatching ? dataBase : nullptr, first_anim, MotionMatchingSolverType::BruteForce),
tree(dataBase->tree), 
index(dataBase, 0, 0),
currentCadr(index.get_lerped_cadr())
{
  if (playerType ==  AnimationPlayerType::StateMachine)
  {
    stateMachine.play_animation(0);
  }
}


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

void AnimationPlayer::set_data_to_IK(const mat4 &t, int i, vec3 foot, vec3 toe, vec3 norm, const char *foot_name, const char *toe_name)
{

  if (onGround & (1u << i))
  {
    if(!ikFoot[i].onGround)
    {  
      ikFoot[i].onGround = true;
      ikFoot[i].footPosition = t * vec4(foot, 1);
      ikFoot[i].toePosition = t * vec4(toe, 1);
      ikFoot[i].footNode = tree.get_child(foot_name);
      ikFoot[i].toeNode = tree.get_child(toe_name);
      ikFoot[i].normal = norm;
    }
  }
  else
  {
    ikFoot[i].onGround = false;
  }
}
void AnimationPlayer::update(Transform &transform, float dt)
{
  dt *= speed;
  
  if (playerType ==  AnimationPlayerType::StateMachine)
  {
    stateMachine.update(dt);
    index = stateMachine.get_current_animation();
  }
  if (playerType ==  AnimationPlayerType::MotionMatching)
  {
    motionMatching.update(dt, inputGoal);
    index = motionMatching.get_index();
  }
  if (playerType ==  AnimationPlayerType::AnimationPlayer)
  {
    index.update(dt);
  }
  float ticks = index.ticks_per_second();
  
  currentCadr = index.get_lerped_cadr();
  rootDeltaTranslation = currentCadr.rootTranslationDelta * ticks;
  rootDeltaRotation = currentCadr.rootRotationDelta * ticks;
  
  if (get_bool_config("UseIK"))
  {
    mat4 t = transform.get_transform();
    vec3 hips = index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::Hips];
    vec3 leftToe = index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::LeftToeBase];
    vec3 rightToe = index.current_index().get_feature().nodes[(int)AnimationFeaturesNode::RightToeBase];
    
    onGround = index.current_index().get_clip().onGround[index.current_index().get_cadr_index()];
    float h = 0;
    vec3 normal, leftToeNormal, rightToeNormal;
    normal = leftToeNormal = rightToeNormal = vec3(0, 1, 0);
    constexpr float ground_value = 0.04f;
    if (!ikFoot[0].onGround && raycast_in_point(t * vec4(leftToe, 1), h, leftToeNormal))
    { 
      h -= ground_value;
      if (on_ground(h))
      {
        onGround |= 1;
      }
      leftToe.y -= h;
    }
    if (!ikFoot[1].onGround && raycast_in_point(t * vec4(rightToe, 1), h, rightToeNormal))
    { 
      h -= ground_value;
      if (on_ground(h))
      {
        onGround |= 2;
      }
      rightToe.y -= h;
    }
    float dh = 0;
    if (raycast_in_point(t * vec4(hips, 1), dh, normal))
    {
      dh -= hips.y;
      dh *= 15;
      rootDeltaTranslation.y += dh;
    }
    
    mat4 inv_t = inverse(t);
    leftToeNormal = inv_t * vec4(leftToeNormal, 0);
    rightToeNormal = inv_t * vec4(rightToeNormal, 0);
    //set_data_to_IK(t, 0, leftFoot, leftToe, leftToeNormal, "LeftFoot", "LeftToeBase");
    //set_data_to_IK(t, 1, rightFoot, rightToe, rightToeNormal, "RightFoot", "RightToeBase");
    
    int hipsNode = tree.get_child("Hips");
    for (int i = 0; i < 2; i++)
    {
      if (ikFoot[i].onGround)
      {
        vec3 footPosition = inv_t * vec4(ikFoot[i].footPosition, 1);
        vec3 toePosition = inv_t * vec4(ikFoot[i].toePosition, 1);
        process_IK(tree, currentCadr, t, footPosition, ikFoot[i].normal, ikFoot[i].footNode, hipsNode);
        process_IK(tree, currentCadr, t, toePosition, vec3(0,1,0), ikFoot[i].toeNode, hipsNode);
        vec3 norm = t * vec4(ikFoot[i].normal, 0);
        draw_arrow(ikFoot[i].toePosition, ikFoot[i].toePosition + norm * 0.3f, vec3(10,0,0), 0.02f, false);
      }
    }
  }
  tree.set_cadr(currentCadr);
  tree.calculate_bone_transforms();
}


void AnimationPlayer::animation_selector(const KeyboardEvent &event)
{
  if (playerType !=  AnimationPlayerType::AnimationPlayer)
    return;
  int anim = index.current_index().get_clip_index();
  if (event.keycode == SDLK_RIGHT)
    anim++;
  if (event.keycode == SDLK_LEFT)
    anim--;
  int n = index.get_data_base()->clips.size();
  anim = (n + anim) % n;
  index.current_index().set_index(anim, 0);
  index.current_index().set_index(anim, 1);
}

AnimationStateMachine *AnimationPlayer::get_state_machine()
{
  return playerType ==  AnimationPlayerType::StateMachine ? &stateMachine : nullptr;
}
MotionMatching *AnimationPlayer::get_motion_matching()
{
  return playerType ==  AnimationPlayerType::MotionMatching ? &motionMatching : nullptr;
}
const MotionMatching *AnimationPlayer::get_motion_matching() const
{
  return playerType ==  AnimationPlayerType::MotionMatching ? &motionMatching : nullptr;
}
AnimationLerpedIndex AnimationPlayer::get_index() const
{
  return index;
}

const AnimationTree &AnimationPlayer::get_tree() const
{
  return tree;
}
