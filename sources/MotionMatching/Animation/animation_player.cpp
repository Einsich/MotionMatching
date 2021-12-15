#include "animation_player.h"
#include "animation_ik.h"
#include <component_editor.h>
#include <resources/resources.h>

AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, AnimationPlayerType playerType):
playerType(playerType), speed(1.f),
index(dataBase, 0, 0), 
motionMatching(dataBase, index, MotionMatchingSolverType::BruteForce),
tree(&dataBase->tree), 
currentCadr(index.get_lerped_cadr()),
dataBase(dataBase)
{
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


void AnimationPlayer::animation_selector(const KeyEventAnyActionKey &event)
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
