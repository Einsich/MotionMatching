#include "ecs/ecs.h"
#include "Time/time.h"
#include "Event/input.h"
#include "PersonController/person_controller.h"
#include "Animation/man_property.h"
#include "Animation/animation_player.h"
#include "Transform/transform.h"
#include "Animation/Test/animation_tester.h"
constexpr float lerp_strength = 4.f;


vec3 get_boost_dt(vec3 speed, float dt, Input &input)
{
  float right = input.get_key(SDLK_d) - input.get_key(SDLK_a);
  float forward = input.get_key(SDLK_w) - input.get_key(SDLK_s);
  //forward *= 1.3f;
  bool onPlace = float_equal(right, 0) && float_equal(forward, 0);
  float run = input.get_key(SDLK_LSHIFT);
  vec3 wantedSpeed(right, 0, forward);
  if (!onPlace)
  {
    wantedSpeed = normalize(wantedSpeed);
    int moveIndex = forward > 0 ? 0 : forward < 0 ? 2 : 1;
    if(run > 0)
    {
      wantedSpeed *= ManProperty::instance->runSpeeds[moveIndex];
    }
    else
    {
      wantedSpeed *= ManProperty::instance->walkSpeeds[moveIndex];
    }
  }
  vec3 dv = (wantedSpeed - speed);
  float dvLength = length(dv);
  if (onPlace && dvLength < 0.1f)
  {
    return dv;
  }
  if (dvLength < 1e-1f)
  {
    return vec3(0.f);
  }
  else
  {
    return normalize(dv) * 3.f * dt;
  }
}

template<typename Callable>
void update_attached_camera(const ecs::EntityId&, Callable);

SYSTEM(ecs::SystemOrder::LOGIC) peson_controller_update(
  AnimationPlayer &animationPlayer,
  PersonController &personController,
  AnimationTester *animationTester,
  Transform &transform) 
{
  
  Input &input = animationTester ? animationTester->testInput : Input::input();
  float dt = Time::delta_time();
  animationPlayer.update(transform, dt);
  personController.simulatedRotation =
   lerp(personController.simulatedRotation, personController.wantedRotation, dt * lerp_strength);


  personController.update_from_speed(animationPlayer, transform, personController.speed + get_boost_dt(personController.speed, dt, input), dt);


  float rotationDelta = personController.wantedRotation - personController.realRotation;
  
  animationPlayer.inputGoal.tags.clear();
  if (input.get_key(SDLK_SPACE) > 0)
  {

  }
  
  constexpr float rotationSpeed = DegToRad * 90;//radians per second
  float T = AnimationTrajectory::timeDelays[AnimationTrajectory::PathLength - 1];
  float rotations[AnimationTrajectory::PathLength];
  if (rotationSpeed * T < abs(rotationDelta))
  {
    float rotationMin = sign(rotationDelta) * rotationSpeed * T;
    float rotationAdd = rotationDelta - rotationMin;
    constexpr float K = 2.f / (AnimationTrajectory::PathLength * (AnimationTrajectory::PathLength - 1));
    constexpr float M = 1.f / AnimationTrajectory::PathLength;
    for (int i = 0; i < AnimationTrajectory::PathLength; i++)
      rotations[i] = rotationMin * M + rotationAdd * (AnimationTrajectory::PathLength - 1 - i) * K;

  }
  else
  {
    for (int i = 0; i < AnimationTrajectory::PathLength; i++)
      rotations[i] = rotationDelta / AnimationTrajectory::PathLength;
  }
  for (int i = 1; i < AnimationTrajectory::PathLength; i++)
    rotations[i] += rotations[i - 1];
  

  vec3 hipsPoint = vec3(0, personController.crouching ? ManProperty::instance->hipsHeightCrouch : ManProperty::instance->hipsHeightStand, 0);
  vec3 prevPoint = hipsPoint;
  vec3 predictedSpeed = personController.speed;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float t = AnimationTrajectory::timeDelays[i];
    float dtdelay = i == 0 ? t : t - AnimationTrajectory::timeDelays[i - 1];
    float x = -rotations[i]; 
    animationPlayer.inputGoal.path.trajectory[i].point = prevPoint + quat(vec3(0,x,0)) * (predictedSpeed * dtdelay);
    animationPlayer.inputGoal.path.trajectory[i].rotation = x;
    prevPoint = animationPlayer.inputGoal.path.trajectory[i].point;
    predictedSpeed += get_boost_dt(predictedSpeed, dtdelay, input);
  }

}

EVENT() controller_mouse_move_handler(
  const ControllerMouseMoveEvent &e,
  PersonController &personController)
{
  float const pixToRad = PI / 180.f * 0.2f;
  personController.wantedRotation += e.e.dx * pixToRad;
}




EVENT() controller_crouch_event_handler(
  const ControllerKeyBoardEvent &e,
  PersonController &personController)
{

  if (e.e.keycode == SDLK_SPACE)
    personController.disableEvents = !personController.disableEvents;
  if (e.e.keycode == SDLK_z)
    personController.crouching = !personController.crouching;
}