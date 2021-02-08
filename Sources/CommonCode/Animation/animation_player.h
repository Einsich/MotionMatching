#pragma once
#include "../Event/input.h"
#include "animation_state_machine.h"
#include "AnimationRender/animation_render.h"
#include "AnimationTree/animation_tree.h"
#include "animation_goal.h"
class AnimationPlayer
{
private:

  float speed = 1.f;
  AnimationStateMachine stateMachine;
  AnimationRender animRender;
  AnimationTree tree;
public:
  vec3 rootDeltaTranslation = vec3(0.f);
  float rootDeltaRotation = 0 ;
  GameObjectPtr gameObject;
  AnimationGoal inputGoal;
  AnimationPlayer(AnimationDataBasePtr dataBase, GameObjectPtr gameObject, int first_anim);
  void update();

  void animation_selector(const KeyboardEvent &event);
  AnimationIndex get_current_animation() const;
  AnimationStateMachine &get_state_machine();
  void render(const Camera& mainCam, const DirectionLight& light);

};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;