#pragma once
#include "Animation/AnimationDatabase/animation_database.h"
class MotionMatchingScene 
{
public:
  AnimationDataBasePtr dataBase;
  MotionMatchingScene(AnimationDataBasePtr dataBase):dataBase(dataBase){}
  MotionMatchingScene() = default;
};
