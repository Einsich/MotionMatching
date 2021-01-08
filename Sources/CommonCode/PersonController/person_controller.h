#pragma once
#include "../GameObject/game_object.h"
#include "../Animation/animation.h"
#include "../Event/input.h"
class TestPersonController
{
private:
  GameObjectPtr character;
  AnimationPlayerPtr animation;
  float rotation = 0;
public:
  TestPersonController(){}
  TestPersonController(GameObjectPtr character, AnimationPlayerPtr animation);

  void update();
  //void start_crouch(const KeyboardEvent &event);
 // void crouch(const KeyboardEvent &event);
 // void jump(const KeyboardEvent &event);
};

