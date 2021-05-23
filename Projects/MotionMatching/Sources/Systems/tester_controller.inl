#include "ecs/ecs.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/animation_player.h"
#include "Engine/transform.h"
#include "Engine/time.h"
#include "Engine/Render/debug_arrow.h"
#include "Animation/person_controller.h"
#include "motion_matching_scene.h"

template<typename Callable>
void get_test_query(Callable);


SYSTEM(ecs::SystemOrder::LOGIC) tester_update(
  ecs::EntityId eid,
  AnimationTester &animationTester)
{
  QUERY() get_test_query([&](MotionMatchingScene &motionMatchingScene)
  {
    float dt = Time::delta_time();
    
    AnimationTest &test = motionMatchingScene.dataBase->tests[animationTester.testInd];

    animationTester.curTime += dt;
    while ((int)test.keyboardEvents.size() > animationTester.keyboardInd && test.keyboardEvents[animationTester.keyboardInd].time <= animationTester.curTime)
    {
      const KeyboardEvent &e = test.keyboardEvents[animationTester.keyboardInd];
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
      ecs::send_event(eid, OnAnimationTestStart(animationTester.testInd));
    }
  });
  
}

EVENT() start_test(
  const OnAnimationTestStart &test,
  AnimationTester &animationTester,
  const vec3 &testOffset,
  Transform &transform,
  PersonController &personController)
{
  animationTester.testInd = test.test;
  animationTester.curTime = 0;
  animationTester.testStartTime = Time::time();
  animationTester.keyboardInd = 0;
  animationTester.mouseMoveInd = 0;
  animationTester.testInput = Input();
  personController.set_pos_rotation(transform, testOffset, 0);
}
