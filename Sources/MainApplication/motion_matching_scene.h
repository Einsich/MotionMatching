#pragma once
#include "CommonCode/component.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Animation/AnimationDatabase/animation_database.h"
class MotionMatchingScene : public Component, public ISaveable
{
private:
  AnimationDataBasePtr dataBase;
public:
  MotionMatchingScene(AnimationDataBasePtr dataBase);
  void save() override;
};

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun);