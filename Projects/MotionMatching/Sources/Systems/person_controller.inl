#include <ecs/ecs.h>
#include <Engine/time.h>
#include <Engine/input.h>
#include "Animation/person_controller.h"
#include "Animation/settings.h"
#include "Animation/animation_player.h"
#include <Engine/transform.h>
#include "Animation/Test/animation_tester.h"
#include <Engine/Render/debug_arrow.h>

constexpr float lerp_strength = 4.f;


vec3 get_wanted_speed(Input &input, bool &onPlace, const ControllerSettings &settings)
{
  float right = input.get_key(SDLK_d) - input.get_key(SDLK_a);
  float forward = input.get_key(SDLK_w) - input.get_key(SDLK_s);
  
  float run = input.get_key(SDLK_LSHIFT);
  
  
  vec3 wantedSpeed =  vec3(right, 0.f, forward);
  float speed = length(wantedSpeed);
  if (speed > 1.f)
    wantedSpeed /= speed;
  onPlace = speed < 0.1f;
  if (!onPlace)
  {
    if(run > 0)
    {
      wantedSpeed *= vec3(settings.runSidewaySpeed,0,forward > 0 ? settings.runForwardSpeed : settings.runBackwardSpeed);
    }
    else
    {
      wantedSpeed *= vec3(settings.walkSidewaySpeed,0,forward > 0 ? settings.walkForwardSpeed : settings.walkBackwardSpeed);
    }
  }
  return wantedSpeed;
}


float lerp_angle(float a_angleA, float a_angleB, float a_t)
{
  
  float da = mod_f(a_angleB - a_angleA, PITWO);

  return a_angleA + (mod_f(2*da,PITWO) - da) * a_t;
}
float safe_2d_angle(vec3 a_, vec3 b_)
{
  vec2 a = vec2(a_.x, a_.z), b = vec2(b_.x, b_.z); 
  float a_l = length(a);
  float b_l = length(b);
  if (a_l < 0.001f || b_l < 0.001f)
    return 0;
  a /= a_l, b /= b_l;
  
  return acos(glm::clamp(dot(a, b), 0.f, 1.f));
}
float safe_2d_signed_angle(vec3 a_, vec3 b_)
{
  vec2 a = vec2(a_.x, a_.z), b = vec2(b_.x, b_.z); 
  float a_l = length(a);
  float b_l = length(b);
  if (a_l < 0.001f || b_l < 0.001f)
    return 0;
  a /= a_l, b /= b_l;
  
  return acos(glm::clamp(dot(a, b), 0.f, 1.f)) * sign(a.x * b.y - a.y * b.x);
}

float rotation_abs(float rotation_delta)
{
  rotation_delta = abs(rotation_delta);
  rotation_delta -= (int)(rotation_delta/PITWO)*PITWO;
  
  rotation_delta = rotation_delta > PI ? PITWO - rotation_delta : rotation_delta;
  return rotation_delta;
}

vec3 apply_root_motion_to_speed(vec3 speed, vec3 root_motion)
{
  return root_motion;
  float speedMagnitude = length(speed);
  if (speedMagnitude > 0.1f)
    speed /= speedMagnitude;
  return root_motion * speed;
}

SYSTEM(ecs::SystemOrder::LOGIC) peson_controller_update(
  AnimationPlayer &animationPlayer,
  PersonController &personController,
  AnimationTester *animationTester,
  Transform &transform,
  int *controllerIndex,
  SettingsContainer &settingsContainer) 
{
  const ControllerSettings &settings = settingsContainer.controllerSettings[controllerIndex ? *controllerIndex : 0].second;

  Input &input = animationTester ? animationTester->testInput : Input::input();
  float dt = Time::delta_time();
  bool onPlace;

  vec3 speed = get_wanted_speed(input, onPlace, settings);
  if (animationTester)
  {
    //debug_log("[%f, %f, %f]", speed.x, speed.y, speed.z);input.get_key(SDLK_w)
    //debug_log("[%f]", input.get_key(SDLK_w));
  }
  
  //bool onlySideway = abs(speed.z) < 0.1f && abs(speed.x) > 0.f;
  bool moveForward = speed.z >= 0.f;
  float sidewayRotation = moveForward ? safe_2d_signed_angle(speed, vec3(0,0,1)) : safe_2d_signed_angle(-speed, vec3(0,0,1));

  float wantedRotation = personController.wantedRotation - sidewayRotation;
  float nextRootRotation = personController.realRotation - animationPlayer.rootDeltaRotation * dt;
  float nextLerpRotation = lerp_angle(personController.realRotation, wantedRotation, dt * settings.rotationRate);
  float desiredOrientation = mod_f(wantedRotation - personController.realRotation, PITWO);
  if (rotation_abs(nextRootRotation - wantedRotation) < rotation_abs(nextLerpRotation - wantedRotation))
  {
    personController.realRotation = mod_f(nextRootRotation, PITWO); 
  }
  else
  {
    personController.realRotation = nextLerpRotation;
  }

  float h = settings.hipsHeightStand;
  if (personController.crouching)
  {
    h = animationPlayer.inputGoal.tags.find(AnimationTag::Idle) != animationPlayer.inputGoal.tags.end() ?
        settings.hipsHeightCrouchIdle : settings.hipsHeightCrouch;
  }
  
  //bool onlyForward = speed.z > 0.f && abs(speed.x) < 0.1f;
  speed = glm::rotateY(speed, -wantedRotation - sidewayRotation);

  auto &trajectory = animationPlayer.inputGoal.path.trajectory;

  vec3 v0 = personController.desiredTrajectory[0] / settings.inertionTime; // inertion time
  v0.y = 0;
  vec3 prevDesiredPoint = vec3(0, h, 0);
  float onPlaceError = 0;
   vec3 desiredSpeed = v0;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float timePercentage = AnimationTrajectory::timeDelays[i] / AnimationTrajectory::timeDelays.back();
    float percentage = (i + 1.f) / AnimationTrajectory::PathLength;

    desiredSpeed = lerp(v0, personController.speed, timePercentage);
    float timeDelay = AnimationTrajectory::timeDelays[i]  - (i == 0 ? 0.f : AnimationTrajectory::timeDelays[i - 1]);
    prevDesiredPoint += timeDelay * desiredSpeed;
    trajectory[i].point = prevDesiredPoint;
    float dangle = onPlace ? lerp_angle(0, -desiredOrientation, percentage) :
      moveForward ? safe_2d_signed_angle(desiredSpeed, glm::rotateY(vec3(0,0,1), -personController.realRotation)) : 
      safe_2d_signed_angle(desiredSpeed, glm::rotateY(vec3(0,0,1), PI-personController.realRotation));
    trajectory[i].rotation = dangle;
    onPlaceError += length(timeDelay * desiredSpeed);
  
  }

  vec3 v1 = (trajectory[0].point - personController.desiredTrajectory[0]) / 
    (AnimationTrajectory::timeDelays[0] - settings.inertionTime);
    v1.y = 0;
  personController.desiredTrajectory[0] += (v1 - v0) * dt;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
    trajectory[i].point = glm::rotateY(trajectory[i].point, personController.realRotation);


  animationPlayer.inputGoal.tags.clear();
  if(personController.crouching)
    animationPlayer.inputGoal.tags.insert(AnimationTag::Crouch);

  float dr = abs(desiredOrientation);
  dr = dr < PI ? dr : PITWO - dr;
  if (onPlaceError < settings.onPlaceMoveError && dr * RadToDeg < settings.onPlaceRotationError)
    animationPlayer.inputGoal.tags.insert(AnimationTag::Idle);

  if (input.get_key(SDLK_SPACE) > 0)
  {

  }
  if (length2(speed) > 0.1f)
  {
    if (safe_2d_angle(speed, v0) * RadToDeg > settings.strafeAngle)
    {
      personController.speed = speed * settings.strafeStartSpeed;
    }
    else
    {
      if (length2(personController.speed) < 0.1f)
        personController.speed = speed * settings.startSpeed;
      else
        personController.speed = lerp(personController.speed, speed, dt * settings.accelerationRate); 
    }
  }
  else
  {
    personController.speed = lerp(personController.speed, speed, dt * settings.decelerationRate);
  }
  personController.simulatedPosition += v0 * dt * transform.get_scale();
  vec3 rootDelta = apply_root_motion_to_speed(speed, animationPlayer.rootDeltaTranslation);


  personController.realPosition = transform.get_position() + transform.get_rotation() * rootDelta * dt;

  vec3 positionDelta = personController.simulatedPosition - personController.realPosition;
  float errorRadius = length(positionDelta);
  if (errorRadius > settings.maxMoveErrorRadius)
  {
    personController.realPosition += positionDelta * (errorRadius-settings.maxMoveErrorRadius)/errorRadius;
  }
  transform.get_position() = personController.realPosition;
  transform.set_rotation(-personController.realRotation); 

  draw_transform(transform);
   
}

EVENT() controller_mouse_move_handler(
  const ControllerMouseMoveEvent &e,
  PersonController &personController,
  const Settings &settings)
{
  float dx = e.e.dx * DegToRad * settings.mouseSensitivity;
  personController.wantedRotation += (settings.mouseInvertXaxis ? 1 : -1) * dx;
}




EVENT() controller_crouch_event_handler(
  const ControllerKeyBoardEvent &e,
  PersonController &personController)
{
  if (e.e.action == KeyAction::Down)
  {
    if (e.e.keycode == SDLK_SPACE)
      personController.disableEvents = !personController.disableEvents;
    if (e.e.keycode == SDLK_z)
      personController.crouching = !personController.crouching;
  }
}

PersonController::PersonController(vec3 position) :
simulatedRotation(0), realRotation(0), wantedRotation(0), angularSpeed(0),
speed(0),
simulatedPosition(position), realPosition(position),
disableEvents(false),
crouching(false),
rotationStrafe(0)
{
  for (vec3&v:desiredTrajectory)
    v = vec3(0.f);
  for (float&r:desiredOrientation)
    r = 0;
}


void PersonController::set_pos_rotation(Transform &transform, vec3 position, float rotation)
{
  realPosition = simulatedPosition = position;
  realRotation = simulatedRotation = rotation;
  transform.get_position() = position;
  transform.set_rotation(realRotation); 
}