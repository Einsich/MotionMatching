#pragma once
#include "animation_player.h"
#include "imgui/imgui.h"
#include "CommonCode/GameObject/game_object.h"

class AnimationDebugRender
{
private:
  GameObjectPtr debugSphere;
  void show_statistics(AnimationFeaturesWeightsPtr weights, const vector<AnimationClip> &animations, const AnimationFeatures &feature);
  void render_spheres(const Camera& mainCam, const DirectionLight& light,const Transform &transform, const AnimationFeatures &feature);
public:
  AnimationDebugRender();
  void show_ui_matching(AnimationPlayerPtr player);
  void render_pose_matching(AnimationPlayerPtr player, const Camera& mainCam, const DirectionLight& light);

};
using AnimationDebugRenderPtr = shared_ptr<AnimationDebugRender>;