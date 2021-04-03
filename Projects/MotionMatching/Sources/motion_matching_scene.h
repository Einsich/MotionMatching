#pragma once
#include "component.h"
#include "GameObject/game_object.h"
#include "Animation/AnimationDatabase/animation_database.h"
#include "Event/input.h"
class MotionMatchingScene : public Component, public ISaveable
{
private:
  AnimationDataBasePtr dataBase;
public:
  MotionMatchingScene(AnimationDataBasePtr dataBase);
  void save() override;
  void toggle_main_camera_handler(const KeyboardEvent &e);
};

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun);