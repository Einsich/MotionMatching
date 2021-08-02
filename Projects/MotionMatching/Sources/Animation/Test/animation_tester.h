#pragma once
#include "Engine/input.h"
#include "Serialization/reflection.h"
class AnimationTester
{
public:
  REFLECT(AnimationTester, (int) (testInd))
  int keyboardInd, mouseMoveInd;
  float testStartTime, curTime;
  Input testInput;
};
