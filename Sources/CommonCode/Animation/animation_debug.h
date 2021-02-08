#pragma once
#include "animation_player.h"
#include "imgui/imgui.h"
#include "CommonCode/GameObject/game_object.h"

class AnimationDebugRender
{
private:
  GameObjectPtr debugSphere;
  AnimationDataBasePtr dataBase;
  AnimationPlayerPtr animPlayer;
  vector<vector<float>> poseMatchingWeights;
  vector<const AnimationFeatures*> targetFeatures;
  Transform transform;
  int besti, bestj;
  float best;
  void show_statistics(AnimationFeaturesWeightsPtr weights, const vector<AnimationClip> &animations, const AnimationFeatures &feature);
  void render_spheres(const Camera& mainCam, const DirectionLight& light,const Transform &transform, const AnimationFeatures &feature);
public:
  AnimationDebugRender(AnimationDataBasePtr dataBase);
  void analyze_pose_matching(AnimationPlayerPtr animPlayer);
  void show_ui_pose_matching();
  void render_pose_matching(const Camera& mainCam, const DirectionLight& light);

};
using AnimationDebugRenderPtr = shared_ptr<AnimationDebugRender>;