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
  bool crouching = false;
public:
  TestPersonController(){}
  TestPersonController(GameObjectPtr character, AnimationPlayerPtr animation);

  void update();
  void crouch(const KeyboardEvent &event);
  void jump(const KeyboardEvent &event);
};

