#pragma once
#include "CommonCode/common.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Light/direction_light.h"
#include "../Animation/AnimationDatabase/animation_database.h"
#include <vector>
class Scene
{
private:
  vector<GameObject*> gameObjects;
  AnimationDataBasePtr dataBase;
  DirectionLight sun;
public:
  void init();
  void update();
  void render();
  void render_ui();
  void exit();

};