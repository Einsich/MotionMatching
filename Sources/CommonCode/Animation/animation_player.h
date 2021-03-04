#pragma once
#include "../Event/input.h"
#include "CommonCode/component.h"
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
class AnimationPlayer : public Component, public IUpdatable
{
private:
  AnimationPlayerType playerType;
  float speed;
  AnimationStateMachine stateMachine;
  MotionMatching motionMatching;
  AnimationTree tree;
  AnimationLerpedIndex index;
  AnimationCadr currentCadr;
  struct IKFoot
  {
    bool onGround = false;
    vec3 footPosition, toePosition, normal;
    int footNode, toeNode;
  };
  IKFoot ikFoot[2];
  void set_data_to_IK(const mat4 &t, int i, vec3 foot, vec3 toe, vec3 norm, const char *foot_name, const char *toe_name);
public:
  uint8 onGround;
  vec3 rootDeltaTranslation = vec3(0.f);
  float rootDeltaRotation = 0;
  AnimationGoal inputGoal;
  AnimationPlayer(AnimationDataBasePtr dataBase, int first_anim, AnimationPlayerType playerType);
  void update() override;

  void animation_selector(const KeyboardEvent &event);
  AnimationStateMachine *get_state_machine();
  MotionMatching *get_motion_matching();
  void render(const Camera& mainCam, const DirectionLight& light);
  AnimationLerpedIndex get_index() const;
  const AnimationTree &get_tree() const;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;