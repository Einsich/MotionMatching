#pragma once

#include "../AnimationDatabase/animation_database.h"
class AnimationTester
{
public:
  AnimationDataBasePtr dataBase;
  int testInd;
  int curPoint;
  float curTime;
  vec3 offset;
  AnimationTester(AnimationDataBasePtr dataBase, int cur_test, vec3 test_offset):
  dataBase(dataBase), testInd(cur_test), curPoint(0), curTime(0), offset(test_offset){}
};
struct OnAnimationTestStart
{
  int test;
  OnAnimationTestStart(int test) : test(test){}
};