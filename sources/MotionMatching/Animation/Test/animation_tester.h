#pragma once
#include "input.h"
#include "serialization/reflection.h"
class AnimationTester
{
public:
  REFLECT(AnimationTester, (int) (testInd))
  int keyboardInd, mouseMoveInd;
  float testStartTime, curTime;
  Input testInput;
};
