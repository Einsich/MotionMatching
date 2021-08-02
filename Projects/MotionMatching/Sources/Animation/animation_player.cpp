#include "animation_player.h"
#include "Engine/time.h"
#include "animation_ik.h"
#include "Engine/Render/debug_arrow.h"
#include "Engine/Physics/physics.h"
#include "config.h"
#include "Engine/Profiler/profiler.h"
AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, string first_anim, AnimationPlayerType playerType):
playerType(playerType), speed(1.f), 
motionMatching(playerType ==  AnimationPlayerType::MotionMatching ? dataBase : nullptr, first_anim, MotionMatchingSolverType::BruteForce),
tree(&dataBase->tree), 
index(dataBase, 0, 0),
currentCadr(index.get_lerped_cadr())
{
  for (uint i = 0; i < dataBase->clips.size(); ++i)
    if (dataBase->clips[i].name == first_anim)
    {
      index = AnimationLerpedIndex(dataBase, i, 0);
      currentCadr = index.get_lerped_cadr();
    }
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

const AnimationTree &AnimationPlayer::get_tree() const
{
  return tree;
}
