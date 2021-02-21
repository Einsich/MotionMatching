#pragma once
#include "CommonCode/GameObject/game_object.h"

class AnimationDebugRender : public Component, public IRenderable, public IUIRenderable
{
private:
  GameObjectPtr debugSphere;
public:
  AnimationDebugRender();
  void ui_render() override;
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;

};