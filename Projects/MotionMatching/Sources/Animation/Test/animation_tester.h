#pragma once
#include "Engine/input.h"
#include "Serialization/serialization.h"
class AnimationTester
{
public:
  int testInd;
  int keyboardInd, mouseMoveInd;
  float testStartTime, curTime;
  Input testInput;
};
struct OnAnimationTestStart
{
  int test;
  OnAnimationTestStart(int test):test(test){}
};
