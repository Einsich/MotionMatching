#pragma once
#include "../Event/input.h"
#include "animation_state_machine.h"
#include "MotionMatching/motion_matching.h"
#include "AnimationRender/animation_render.h"
#include "AnimationTree/animation_tree.h"
#include "animation_goal.h"
enum class AnimationPlayerType
{
  StateMachine,
  MotionMatching, 
  AnimationPlayer
};
class AnimationPlayer
{
private:
  AnimationPlayerType playerType;
  float speed;
  AnimationStateMachine stateMachine;
  MotionMatching motionMatching;
  AnimationRender animRender;
  AnimationTree tree;
  AnimationLerpedIndex index;
public:
  vec3 rootDeltaTranslation = vec3(0.f);
  float rootDeltaRotation = 0;
  GameObjectPtr gameObject;
  AnimationGoal inputGoal;
  AnimationPlayer(AnimationDataBasePtr dataBase, GameObjectPtr gameObject, int first_anim, AnimationPlayerType playerType);
  void update();

  void animation_selector(const KeyboardEvent &event);
  AnimationStateMachine *get_state_machine();
  MotionMatching *get_motion_matching();
  void render(const Camera& mainCam, const DirectionLight& light);
  AnimationLerpedIndex get_index() const;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;