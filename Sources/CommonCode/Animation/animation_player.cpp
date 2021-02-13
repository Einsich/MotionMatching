#include "animation_player.h"
#include "CommonCode/Time/time.h"


AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, GameObjectPtr gameObject, int first_anim, bool useStateMachine):
useStateMachine(useStateMachine), speed(1.f), 
stateMachine(!useStateMachine ? nullptr : dataBase), 
motionMatching(useStateMachine ? nullptr : dataBase, first_anim, MotionMatchingSolverType::BruteForce),
animRender(gameObject), tree(dataBase->tree), 
gameObject(gameObject)
{
  if (useStateMachine)
  {
    stateMachine.play_animation(first_anim);
  }
  else
  {

  }
}

void AnimationPlayer::update()
{
  float dt = speed * Time::delta_time();
  AnimationLerpedIndex index;
  if (useStateMachine)
  {
    stateMachine.update(dt);
    index = stateMachine.get_current_animation();
  }
  else
  {
    motionMatching.update(dt, inputGoal);
    index = motionMatching.get_index();
  }
  float ticks = index.ticks_per_second();
  
  AnimationCadr cadr = index.get_lerped_cadr();
  rootDeltaTranslation = cadr.rootTranslationDelta * ticks;
  rootDeltaRotation = cadr.rootRotationDelta * ticks;
  tree.set_cadr(cadr);
}


void AnimationPlayer::animation_selector(const KeyboardEvent &event)
{
  return;
  static int anim = 0;
  if (event.keycode == SDLK_RIGHT)
    anim++;
  if (event.keycode == SDLK_LEFT)
    anim--;
  stateMachine.play_animation(anim);  
}

AnimationStateMachine &AnimationPlayer::get_state_machine()
{
  return stateMachine;
}
MotionMatching &AnimationPlayer::get_motion_matching()
{
  return motionMatching;
}
void AnimationPlayer::render(const Camera& mainCam, const DirectionLight& light)
{
  animRender.render(mainCam, light, tree);
}