#include "ecs/ecs.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/animation_player.h"
#include "Engine/transform.h"
#include "Engine/time.h"
#include "Animation/person_controller.h"
#include "Animation/settings.h"


SYSTEM(ecs::SystemOrder::LOGIC) tester_update(
  ecs::EntityId eid,
  AnimationTester &animationTester,
  AnimationPlayer &animationPlayer)
{
    float dt = Time::delta_time();
    
    AnimationTest &test = animationPlayer.dataBase->tests[animationTester.testInd];

    animationTester.curTime += dt;
    while ((int)test.keyboardEvents.size() > animationTester.keyboardInd && test.keyboardEvents[animationTester.keyboardInd].time <= animationTester.curTime)
    {
      const KeyEventAnyActionKey &e = test.keyboardEvents[animationTester.keyboardInd];
      SDL_KeyboardEvent event;
      memset(&event, 0, sizeof(event));
      event.keysym.sym = e.keycode;
      if (e.action == KeyAction::Press)
        event.repeat = true;
      else 
        event.state = e.action == KeyAction::Down ? SDL_PRESSED : SDL_RELEASED;
      animationTester.testInput.event_process(event, Time::time());
      ecs::send_event_immediate(eid, ControllerKeyBoardEvent(e));
      animationTester.keyboardInd++;
      
    }
    while ((int)test.mouseMoveEvents.size() > animationTester.mouseMoveInd && test.mouseMoveEvents[animationTester.mouseMoveInd].time <= animationTester.curTime)
    {
      const MouseMoveEvent &e = test.mouseMoveEvents[animationTester.mouseMoveInd];
      SDL_MouseMotionEvent event;

      event.x = e.x , event.y = e.y,
      event.xrel = e.dx , event.yrel = e.dy;
  
      animationTester.testInput.event_process(event, Time::time());
      ecs::send_event_immediate(eid, ControllerMouseMoveEvent(e));
      animationTester.mouseMoveInd++;
    }
    if (test.totalTime <= animationTester.curTime)
    {
      ecs::send_event(eid, ecs::OnEntityCreated());
    }
}

EVENT(ecs::SystemTag::GameEditor) start_test(
  const ecs::OnEntityCreated &,
  AnimationTester &animationTester,
  const AnimationPlayer &animationPlayer,
  Transform &transform,
  PersonController &personController,
  const Settings &settings)
{
  float edge = sqrt((float)settings.testCount) * settings.testDensity;
  int maxTest = animationPlayer.dataBase->tests.size();
  animationTester.testInd = rand() % maxTest;
  animationTester.curTime = 0;
  animationTester.testStartTime = Time::time();
  animationTester.keyboardInd = 0;
  animationTester.mouseMoveInd = 0;
  animationTester.testInput = Input(false);
  float x = edge * rand() / RAND_MAX;
  float y = edge * rand() / RAND_MAX;
  personController.set_pos_rotation(transform, vec3(x, 0, y), 0);
}

SYSTEM(ecs::SystemTag::Game) test_count(
  vector<ecs::EntityId> &testers,
  Settings &settings)
{
  settings.testCount = glm::clamp(settings.testCount, 0, 10000);
  int d = settings.testCount - testers.size();
  if (d > 0)
  {
    testers.reserve(settings.testCount);
    for (int i = 0; i < d; ++i)
    {
      testers.emplace_back(ecs::create_entity("anim_tester"));
    }
  }
  if (d < 0)
  {
    auto from = testers.begin() + settings.testCount;
    auto to = testers.end();
    for (auto it = from; it != to; ++it)
    {
      ecs::destroy_entity(*it);
    }
    testers.erase(from, to);
  }
}