#include "ecs/ecs.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/animation_player.h"
#include "Transform/transform.h"
#include "GameObject/game_object.h"
#include "Time/time.h"
#include "Components/DebugTools/debug_arrow.h"
#include "PersonController/person_controller.h"



SYSTEM(ecs::SystemOrder::LOGIC) tester_update(
  ecs::EntityId eid,
  AnimationTester &animationTester,
  AnimationPlayer &animationPlayer,
  Transform &transform,
  PersonController &personController)
{
  float dt = Time::delta_time();
  animationPlayer.update(transform, dt);
  AnimationGoal &goal = animationPlayer.inputGoal;
  AnimationTest &test = animationTester.dataBase->test[animationTester.testInd];
  TestPoint testPoint = test.get_lerped_point(animationTester.curPoint, animationTester.curTime);
  vec3 p0 = testPoint.point;
  p0.y = 0;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    TestPoint p = test.get_lerped_point(animationTester.curPoint, animationTester.curTime + AnimationTrajectory::timeDelays[i]);
    goal.path.trajectory[i].point = p.point - p0;
    goal.path.trajectory[i].rotation = p.rotation;
  }
  personController.update_from_speed(animationPlayer, transform, testPoint.velocity, dt);

  animationTester.curTime += dt;
  if (test.points[animationTester.curPoint].time <= animationTester.curTime)
  {
    if (animationTester.curPoint + 1 < (int)test.points.size())
    {
      animationTester.curPoint++;
    }
    else
    {
      ecs::send_event(eid, OnAnimationTestStart(animationTester.testInd));
    }
  }
}

EVENT() start_test(
  const OnAnimationTestStart &test,
  AnimationTester &animationTester,
  Transform &transform,
  PersonController &personController)
{

  animationTester.testInd = test.test >= 0 ? test.test % animationTester.dataBase->test.size() : animationTester.testInd;
  animationTester.curPoint = 0;
  animationTester.curTime = 0;
  personController.set_pos_rotation(transform, animationTester.offset, 0);
}
/*
void AnimationTester::render(const Camera&, const DirectionLight&, bool)
{
  AnimationPlayer * player = game_object()->get_component<AnimationPlayer>();
  if (!player)
    return;
  AnimationLerpedIndex index = player->get_motion_matching() ? player->get_motion_matching()->get_index() : player->get_index();
  AnimationTrajectory trajectory = index.first.get_trajectory();

  Transform* transform = game_object()->get_component<Transform>();
  if (!transform)
    return;
  mat4 transformation = transform->get_transform();
  AnimationTest &test = dataBase->test[testInd];
  for (int i = 0; i < (int)test.points.size(); i++)
  {
    vec3 p0 = test.points[i].point + offset;
    vec3 d = rotateY(vec3(0,0,1), test.points[i].rotation);
    d *= 0.1f;
    vec3 color = i == curPoint ? vec3(2,0,0) : vec3(0,2,0);
    draw_arrow(p0, p0 + d, color, 0.02f, false);
  }
  
  constexpr float dirLength = 0.3f;
  constexpr vec3 colors[2] = {vec3(0,1,0), vec3(1,0,0)};
  constexpr float lenghts[2] = {0.3f, 0.3f};

  std::array<TrajectoryPoint,AnimationTrajectory::PathLength> *trajectories[2] = {&trajectory.trajectory, &player->inputGoal.path.trajectory};
  for(int i = 0; i < 2; i++)
  {
    for (TrajectoryPoint &p: *trajectories[i])
    {
      vec3 v = vec3(transformation * vec4(p.point, 1.f));
      vec3 w = vec3(transformation * vec4(quat(vec3(0, p.rotation, 0)) * vec3(0, 0, dirLength * lenghts[i]), 0.f));
      draw_arrow(v, v + w, colors[i], 0.02f, false);
    }
  }
}
*/