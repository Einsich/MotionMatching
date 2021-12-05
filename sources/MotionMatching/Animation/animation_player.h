#pragma once
#include <input.h>
#include "MotionMatching/motion_matching.h"
#include "AnimationTree/animation_tree.h"
#include <serialization/iserializable.h>
enum class AnimationPlayerType
{
  MotionMatching, 
  AnimationPlayer
};
class Transform;
class AnimationPlayer final : ISerializable
{
  public:
  AnimationPlayerType playerType;
  float speed;
  AnimationLerpedIndex index;
  MotionMatching motionMatching;
  AnimationTree tree;
  vector<int> treeBoneToMesh;
  struct IKFoot
  {
    bool onGround = false;
    vec3 footPosition, toePosition, normal;
    int footNode, toeNode;
  };
  IKFoot ikFoot[2];
  void set_data_to_IK(const mat4 &t, int i, vec3 foot, vec3 toe, vec3 norm, const char *foot_name, const char *toe_name);

  AnimationCadr currentCadr;
  uint8 onGround;
  vec3 rootDeltaTranslation = vec3(0.f);
  float rootDeltaRotation = 0;
  AnimationGoal inputGoal;
  AnimationDataBasePtr dataBase;
  AnimationPlayer(AnimationDataBasePtr dataBase, AnimationPlayerType playerType);
  AnimationPlayer() = default;

  void animation_selector(const KeyEventAnyActionKey &event);
  MotionMatching *get_motion_matching();
  const MotionMatching *get_motion_matching() const;
  AnimationLerpedIndex get_index() const;

  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override; 
};