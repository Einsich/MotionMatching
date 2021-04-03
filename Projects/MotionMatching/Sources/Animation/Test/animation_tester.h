#pragma once

#include "../AnimationDatabase/animation_database.h"
#include "component.h"
class AnimationTester : public Component, public IUpdatable, public IRenderable
{
private:
  AnimationDataBasePtr dataBase;
  int testInd;
  int curPoint;
  float curTime;
  float curRotation, wantedRotation;
  vec3 offset;
public:
  AnimationTester(AnimationDataBasePtr dataBase, int cur_test, vec3 test_offset);
  void update() override;
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;
  void start_test(int test = -1);
};