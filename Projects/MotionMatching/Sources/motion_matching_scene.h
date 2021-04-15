#pragma once
#include "Animation/AnimationDatabase/animation_database.h"
#include "Light/direction_light.h"
class MotionMatchingScene 
{
public:
  AnimationDataBasePtr dataBase;
  MotionMatchingScene(AnimationDataBasePtr dataBase):dataBase(dataBase){}
};
struct SceneRender
{
  DirectionLight sun;
};