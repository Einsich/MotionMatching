#include "animation_player.h"
#include "CommonCode/Time/time.h"


AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, GameObjectPtr gameObject, int first_anim):
stateMachine(dataBase), animRender(gameObject), tree(dataBase->tree), gameObject(gameObject)
{
  stateMachine.play_animation(first_anim);
}

void AnimationPlayer::update()
{
  float dt = speed * Time::delta_time();
  stateMachine.update(dt);
  AnimationCadr cadr = stateMachine.get_lerped_cadr();
  dt = stateMachine.ticksPerSecond();
  rootDeltaTranslation = cadr.rootTranslationDelta * dt;
  rootDeltaRotation = cadr.rootRotationDelta * dt;
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

AnimationIndex AnimationPlayer::get_current_animation() const
{
  return stateMachine.get_current_animation();
}

AnimationStateMachine &AnimationPlayer::get_state_machine()
{
  return stateMachine;
}
void AnimationPlayer::render(const Camera& mainCam, const DirectionLight& light)
{
  animRender.render(mainCam, light, tree);
}