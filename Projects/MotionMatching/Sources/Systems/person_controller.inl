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

float get_drotation(float &s, float r, float dt)
{
  constexpr float angularW = 360 * DegToRad;
  constexpr float maxAngularSpeed = 90 * DegToRad;
  constexpr float littleAngle = 5 * DegToRad;


  float rotationSigh = glm::sign(r);

  float dRotationAbs = r * rotationSigh;
  
  if (dRotationAbs <= littleAngle)
  {
    s = 0;
    return 0;
  }

  float v = sqrt((2 * dRotationAbs * angularW + s*s)*0.5f);
  if (v <= s)
  {
    s -= angularW * dt;
  }
  else
  if (s <= maxAngularSpeed)
  {
    s += angularW * dt;
  }


  return s * rotationSigh * dt;
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
  
  personController.simulatedRotation += get_drotation(personController.angularSpeed, personController.wantedRotation - personController.simulatedRotation, dt);
  

  personController.update_from_speed(animationPlayer, transform, personController.speed + get_boost_dt(personController.speed, dt, input), dt);


  float rotationDelta = personController.wantedRotation - personController.realRotation;
  
  animationPlayer.inputGoal.tags.clear();
  if (input.get_key(SDLK_SPACE) > 0)
  {

  }
  
  

  vec3 hipsPoint = vec3(0, personController.crouching ? ManProperty::instance->hipsHeightCrouch : ManProperty::instance->hipsHeightStand, 0);
  vec3 prevPoint = hipsPoint;
  vec3 predictedSpeed = personController.speed;
  float predictedRotationSpeed = personController.angularSpeed;
  float predictedRotation = personController.simulatedRotation;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float t = AnimationTrajectory::timeDelays[i];
    float dtdelay = i == 0 ? t : t - AnimationTrajectory::timeDelays[i - 1];

    predictedRotation += get_drotation(predictedRotationSpeed, personController.wantedRotation - predictedRotation, dtdelay);

    float x = -predictedRotation + personController.simulatedRotation; 
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