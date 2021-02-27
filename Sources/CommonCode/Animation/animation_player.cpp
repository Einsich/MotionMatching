#include "animation_player.h"
#include "CommonCode/Time/time.h"
#include "animation_ik.h"
#include "CommonCode/Components/DebugTools/debug_arrow.h"
#include "CommonCode/Physics/physics.h"
AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, int first_anim, AnimationPlayerType playerType):
playerType(playerType), speed(1.f), 
stateMachine(playerType ==  AnimationPlayerType::StateMachine ? dataBase : nullptr), 
motionMatching(playerType ==  AnimationPlayerType::MotionMatching ? dataBase : nullptr, first_anim, MotionMatchingSolverType::BruteForce),
tree(dataBase->tree), 
index(dataBase, first_anim, 0, first_anim, 1)
{
  if (playerType ==  AnimationPlayerType::StateMachine)
  {
    stateMachine.play_animation(first_anim);
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
  return -0.5f <= h && h <= 0.05;
}

void AnimationPlayer::update()
{
  float dt = speed * Time::delta_time();
  
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
  
  AnimationCadr cadr = index.get_lerped_cadr();
  rootDeltaTranslation = cadr.rootTranslationDelta * ticks;
  rootDeltaRotation = cadr.rootRotationDelta * ticks;

  tree.set_cadr(cadr);
  
  Transform* transform = gameObject->get_component<Transform>();
  if (transform)
  {
    mat4 t = transform->get_transform();
    vec3 hips = index.first.get_feature().features[(int)AnimationFeaturesNode::Hips];
    vec3 leftToe = index.first.get_feature().features[(int)AnimationFeaturesNode::LeftToeBase];
    vec3 rightToe = index.first.get_feature().features[(int)AnimationFeaturesNode::RightToeBase];
    
    onGround = index.first.get_clip().onGround[index.first.get_cadr_index()];
    float h = 0;
    vec3 normal, leftToeNormal, rightToeNormal;
    normal = leftToeNormal = rightToeNormal = vec3(0, 1, 0);
    if (raycast_in_point(t * vec4(leftToe, 1), h, leftToeNormal))
    { 
      if (on_ground(h))
      {
        onGround |= 1;
        vec3 p = t * vec4(leftToe, 1);
        draw_arrow(p, p + leftToeNormal * 0.3f, vec3(10,0,0), 0.02f, false);
      }
      leftToe.y -= h;
    }
    if (raycast_in_point(t * vec4(rightToe, 1), h, rightToeNormal))
    { 
      if (on_ground(h))
      {
        onGround |= 2;
        vec3 p = t * vec4(rightToe, 1);
        draw_arrow(p, p + rightToeNormal * 0.3f, vec3(10,0,0), 0.02f, false);
      }
      rightToe.y -= h;
    }
    float dh = 0;
    if (raycast_in_point(t * vec4(hips, 1), dh, normal))
    {
      dh -= hips.y;
      dh *= 5;
      rootDeltaTranslation.y += dh;
    }
    mat4 inv_t = inverse(t);
    leftToeNormal = inv_t * vec4(leftToeNormal, 0);
    rightToeNormal = inv_t * vec4(rightToeNormal, 0);
    if (onGround & 1)
      process_IK(tree, cadr, t, leftToe, leftToeNormal, tree.get_child("LeftToeBase"), tree.get_child("Hips"));
    if (onGround & 2)
      process_IK(tree, cadr, t, rightToe, rightToeNormal, tree.get_child("RightToeBase"), tree.get_child("Hips"));
  }
  tree.calculate_bone_transforms();
}


void AnimationPlayer::animation_selector(const KeyboardEvent &event)
{
  if (playerType !=  AnimationPlayerType::AnimationPlayer)
    return;
  int anim = index.first.get_clip_index();
  if (event.keycode == SDLK_RIGHT)
    anim++;
  if (event.keycode == SDLK_LEFT)
    anim--;
  int n = index.get_data_base()->clips.size();
  anim = (n + anim) % n;
  index.first.set_index(anim, 0);
  index.second.set_index(anim, 1);
}

AnimationStateMachine *AnimationPlayer::get_state_machine()
{
  return playerType ==  AnimationPlayerType::StateMachine ? &stateMachine : nullptr;
}
MotionMatching *AnimationPlayer::get_motion_matching()
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
