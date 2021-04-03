#include "animation_tester.h"
#include "../animation_player.h"
#include "Transform/transform.h"
#include "GameObject/game_object.h"
#include "Time/time.h"
#include "Components/DebugTools/debug_arrow.h"
AnimationTester::AnimationTester(AnimationDataBasePtr dataBase, int cur_test, vec3 test_offset):
dataBase(dataBase), testInd(cur_test), curPoint(0), curTime(0), curRotation(0), wantedRotation(0), offset(test_offset)
{

}
void AnimationTester::update()
{
  AnimationPlayer* player = game_object()->get_component<AnimationPlayer>();
  Transform* transform = game_object()->get_component<Transform>();
  if (!player || !transform)
    return;

  float dt = Time::delta_time();
  AnimationGoal &goal = player->inputGoal;
  AnimationTest &test = dataBase->test[testInd];
  vec3 p0 = test.get_lerped_point(curPoint, curTime).point;
  p0.y = 0;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    TestPoint p = test.get_lerped_point(curPoint, curTime + AnimationTrajectory::timeDelays[i]);
    goal.path.trajectory[i].point = p.point - p0;
    goal.path.trajectory[i].rotation = p.rotation;
  }
  transform->get_position() -= 
  (player->rootDeltaTranslation.z * transform->get_forward() + 
  player->rootDeltaTranslation.y * transform->get_up()+ 
  -player->rootDeltaTranslation.x * transform->get_right()) * dt ;
  
  curRotation += player->rootDeltaRotation;
  //transform->set_rotation(curRotation); 

  curTime += dt;
  if (test.points[curPoint].time <= curTime)
  {
    if (curPoint + 1 < (int)test.points.size())
    {
      curPoint++;
    }
    else
    {
      start_test(testInd + 1);
    }
  }
}
void AnimationTester::start_test(int test)
{
  Transform* transform = game_object()->get_component<Transform>();
  if (!transform)
    return;
  testInd = test >= 0 ? test % dataBase->test.size() : testInd;
  curPoint = 0;
  curTime = 0;
  curRotation = wantedRotation = 0;
  transform->get_position() = offset;
  transform->set_rotation(curRotation); 
}
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