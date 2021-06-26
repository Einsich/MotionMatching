#pragma once
#include "Animation/AnimationDatabase/animation_database.h"
#include "Engine/Render/render.h"
class MotionMatchingScene 
{
public:
  AnimationDataBasePtr dataBase;
  MotionMatchingScene(AnimationDataBasePtr dataBase):dataBase(dataBase){}
  MotionMatchingScene() = default;
};
