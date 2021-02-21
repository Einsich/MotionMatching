#include "animation_player.h"
#include "CommonCode/Time/time.h"


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
