#pragma once
#include "component.h"
#include "GameObject/game_object.h"
#include "Animation/AnimationDatabase/animation_database.h"
class MotionMatchingScene : public Component, public ISaveable
{
private:
  AnimationDataBasePtr dataBase;
public:
  MotionMatchingScene(AnimationDataBasePtr dataBase);
  void save() override;
};

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun);