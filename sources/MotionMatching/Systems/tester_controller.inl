#include <ecs.h>
#include "Animation/Test/animation_tester.h"
#include "Animation/Test/animation_test.h"
#include <transform.h>
#include <application/time.h>
#include "Animation/person_controller.h"
#include "Animation/settings.h"

template<typename Callable>
void get_tests(Callable);
template<typename Callable>
void get_tests2(Callable);

SYSTEM(stage=act) tester_update(
  ecs::EntityId eid,
  int &testerSeed,
  AnimationTester &animationTester)
{
  QUERY()get_tests([&](const ecs::vector<AnimationTest> &tests)
  {
    float dt = Time::delta_time();
    
    const AnimationTest &test = tests[animationTester.testInd];

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
      testerSeed+=9687;
      ecs::send_event(eid, ecs::OnEntityCreated());
    }
  });
}

EVENT(scene=game, editor) start_test(
  const ecs::OnEntityCreated &,
  AnimationTester &animationTester,
  Transform &transform,
  PersonController &personController,
  int testerSeed,
  const Settings &settings)
{
  QUERY()get_tests2([&](const ecs::vector<AnimationTest> &tests)
  {
    srand(testerSeed);
    float edge = sqrt((float)settings.testCount) * settings.testDensity;
    int maxTest = tests.size();
    animationTester.testInd = rand() % maxTest;
    animationTester.curTime = rand_float(0, tests[animationTester.testInd].totalTime);
    animationTester.testStartTime = Time::time();
    animationTester.keyboardInd = 0;
    animationTester.mouseMoveInd = 0;
    animationTester.testInput = Input(false);
    float x = edge * rand() / RAND_MAX;
    float y = edge * rand() / RAND_MAX;
    personController.set_pos_rotation(transform, vec3(x, 0, y), 0);
  });
}

SYSTEM(scene=game) test_count(
  ecs::vector<ecs::EntityId> &testers,
  ecs::vector<AnimationTest> &tests,
  Settings &settings)
{
  settings.testCount = glm::clamp(settings.testCount, 0, 10000);
  int d = settings.testCount - testers.size();
  if (d > 0)
  {
    int n = testers.size();
    testers.reserve(settings.testCount);
    const ecs::Template *testerTmpl = ecs::get_template("tester_char");
    for (int i = 0; !tests.empty() && i < d; ++i)
    {
      testers.emplace_back(ecs::create_entity(testerTmpl, {{"testerSeed", n + i}}));
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