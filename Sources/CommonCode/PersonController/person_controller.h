#pragma once
#include "../Event/input.h"
#include "CommonCode/component.h"
class TestPersonController : public Component, public IUpdatable
{
private:
  float rotation = 0, wantedRotation = 0;
  bool crouching = false;
public:

  void update() override;
  void crouch(const KeyboardEvent &event);
  void jump(const KeyboardEvent &event);
  void rotate(const KeyboardEvent &event);
};

