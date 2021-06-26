#pragma once
#include "Engine//input.h"
#include "animation_state_machine.h"
#include "MotionMatching/motion_matching.h"
#include "AnimationTree/animation_tree.h"
#include "animation_goal.h"
enum class AnimationPlayerType
{
  StateMachine,
  MotionMatching, 
  AnimationPlayer
};
class Transform;
class AnimationPlayer
{
  public:
  AnimationPlayerType playerType;
  float speed;
  AnimationStateMachine stateMachine;
  MotionMatching motionMatching;
  AnimationTree tree;
  struct IKFoot
  {
    bool onGround = false;
    vec3 footPosition, toePosition, normal;
    int footNode, toeNode;
  };
  IKFoot ikFoot[2];
  void set_data_to_IK(const mat4 &t, int i, vec3 foot, vec3 toe, vec3 norm, const char *foot_name, const char *toe_name);

  AnimationLerpedIndex index;
  AnimationCadr currentCadr;
  uint8 onGround;
  vec3 rootDeltaTranslation = vec3(0.f);
  float rootDeltaRotation = 0;
  AnimationGoal inputGoal;
  AnimationPlayer(AnimationDataBasePtr dataBase, string first_anim, AnimationPlayerType playerType);
  AnimationPlayer() = default;

  void animation_selector(const KeyboardEvent &event);
  AnimationStateMachine *get_state_machine();
  MotionMatching *get_motion_matching();
  const MotionMatching *get_motion_matching() const;
  AnimationLerpedIndex get_index() const;
  const AnimationTree &get_tree() const;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;