#pragma once
#include "CommonCode/common.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Light/direction_light.h"
#include "../PersonController/person_controller.h"
#include <vector>
class Scene
{
private:
  vector<GameObjectPtr> gameObjects;
  AnimationPlayerPtr animPlayer;
  DirectionLight sun;
  TestPersonController personController;
public:
  void init();
  void update();
  void render();
  void render_ui();
  void exit();

};