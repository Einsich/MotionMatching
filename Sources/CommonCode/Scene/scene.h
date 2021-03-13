#pragma once
#include "CommonCode/common.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Light/direction_light.h"
#include <vector>
class Scene
{
private:
  vector<GameObjectPtr> gameObjects;
  DirectionLight sun;
public:
  void init(void (vector<GameObjectPtr>&, DirectionLight&));
  void update();
  void render();
  void render_ui();
  void exit();

};