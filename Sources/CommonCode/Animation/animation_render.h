#pragma once
#include "AnimationTree/animation_tree.h"
#include "animation_channel.h"
#include "bone_render.h"
#include "../GameObject/game_object.h"
#include "../Light/direction_light.h"
#include "../Camera/camera.h"
class AnimationRender
{
private:
  BoneRender boneRender;
  vector<mat4> curTransform;
  GameObjectPtr gameObject;
  bool renderBones;
public:
  AnimationRender(GameObjectPtr gameObject);
  void render(const Camera& mainCam, const DirectionLight& light, const AnimationTree &tree);
};
