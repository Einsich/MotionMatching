#pragma once
#include "CommonCode/common.h"
#include "CommonCode/math.h"
#include "animation_channel.h"
#include "animation_tree.h"
#include "bone_render.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/Event/input.h"
#include <map>


class Animation: public ISerializable
{
private:

public:
  uint duration;
  float ticksPerSecond;
  string name;
  map<string, AnimationChannel> channels;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;

};

class AnimationPlayer: public ISerializable
{
private:
  uint curCadr = 0, curAnim = 0;
  float speed = 1.f, curT = 0;
  mat4 rootTransform;
  BoneRender boneRender;
  void CalculateBonesTransform(AnimationNode &node, mat4 parent, int d);
public:
  AnimationTree animationTree;
  vector<Animation> animations;
  vector<mat4> curTransform;
  GameObjectPtr gameObject;
  void PlayNextCadr();
  void render(const Camera& mainCam, const DirectionLight& light);
  int cadr_count();
  void play_animation(int anim_index);
  void animation_selector(const KeyboardEvent &event);

  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;