#include "ecs/ecs.h"
#include "Engine/time.h"
#include "Engine/input.h"
#include "Animation/person_controller.h"
#include "Animation/settings.h"
#include "Animation/animation_player.h"
#include "Engine/transform.h"
#include "Animation/Test/animation_tester.h"
#include "Engine/Render/debug_arrow.h"
constexpr float lerp_strength = 4.f;


vec3 get_wanted_speed(Input &input, bool &onPlace)
{
  float right = input.get_key(SDLK_d) - input.get_key(SDLK_a);
  float forward = input.get_key(SDLK_w) - input.get_key(SDLK_s);
  
  float run = input.get_key(SDLK_LSHIFT);
  vec3 wantedSpeed(right, 0, forward);
  float speed = length(wantedSpeed);
  if (speed > 1.f)
    wantedSpeed /= speed;
  onPlace = speed < 0.1f;
  if (!onPlace)
  {
    int moveIndex = forward > 0 ? 0 : forward < 0 ? 2 : 1;
    if(run > 0)
      wantedSpeed *= Settings::runSpeeds[moveIndex];
    else
      wantedSpeed *= Settings::walkSpeeds[moveIndex];
  }
  return wantedSpeed;
}

float get_drotation(float &s, float r, float dt, int &strafe)
{
  constexpr float angularW = 90 * DegToRad;
  constexpr float maxAngularSpeed = 60 * DegToRad;
  constexpr float littleAngle = 5 * DegToRad;
  constexpr float middleAngle = 60 * DegToRad;
  constexpr float rightAngle = 90 * DegToRad;

  float rotationSigh = glm::sign(r);

  float dRotationAbs = r * rotationSigh;
  
  if (dRotationAbs <= littleAngle)
  {
    strafe = 0;
    s = 0;
    return 0;
  }
  float w = angularW;
  float maxSpeed = maxAngularSpeed;
  if (dRotationAbs > middleAngle || strafe >= 1)
  {
    if (dRotationAbs > rightAngle || strafe == 2)
    {
      strafe = 2;
      w *= 4;
      maxSpeed *= 4;
    }
    else
    {
      strafe = 1;
      w *= 2;
      maxSpeed *= 2;
    }
  }


  float v = sqrt((2 * dRotationAbs * w + s*s)*0.5f);
  if (v <= s)
  {
    s -= w * dt;
  }
  else
  if (s <= maxSpeed)
  {
    s += w * dt;
  }


  return s * rotationSigh * dt;
}

float mod_f(float x, float n)
{
  x = x - (int)(x/n) * n;
  return x;
}
float lerp_angle(float a_angleA, float a_angleB, float a_t)
{
  
  float da = mod_f(a_angleB - a_angleA, PITWO);

  return a_angleA + (mod_f(2*da,PITWO) - da) * a_t;
}
  constexpr float maxErrorRadius = 0.55f;

float rotation_abs(float rotation_delta)
{
  rotation_delta = abs(rotation_delta);
  rotation_delta -= (int)(rotation_delta/PITWO)*PITWO;
  
  rotation_delta = rotation_delta > PI ? PITWO - rotation_delta : rotation_delta;
  return rotation_delta;
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
  bool onPlace;
  personController.simulatedRotation += get_drotation(personController.angularSpeed, personController.wantedRotation - personController.simulatedRotation, dt, personController.rotationStrafe);
  

  vec3 speed = get_wanted_speed(input, onPlace);
  float MoveRate = Settings::moveRate, TurnRate = Settings::rotationRate;
  
  float nextRotation = personController.realRotation - animationPlayer.rootDeltaRotation * dt;
  
  if (rotation_abs(nextRotation - personController.wantedRotation) < rotation_abs(personController.realRotation - personController.wantedRotation))
  {
    personController.realRotation = mod_f(nextRotation, PITWO); 
  }
  else
  {
    personController.realRotation = lerp_angle(personController.realRotation, personController.wantedRotation, dt);//1.f - exp(-TurnRate * dt * 0.1f));
  }

  personController.simulatedPosition += transform.get_rotation() * glm::rotateY(speed * dt, -(personController.wantedRotation-personController.realRotation));
  
  vec3 rootDelta = animationPlayer.rootDeltaTranslation;

  personController.simulatedPosition =
  personController.realPosition = transform.get_position() + transform.get_rotation() * rootDelta * dt;

  vec3 positionDelta = personController.simulatedPosition - personController.realPosition;
  float errorRadius = length2(positionDelta);
  if (errorRadius > maxErrorRadius * maxErrorRadius)
  {
    errorRadius = sqrt(errorRadius);
    personController.realPosition += positionDelta * (errorRadius-maxErrorRadius)/errorRadius;
  }
  transform.get_position() = personController.realPosition;
  transform.set_rotation(-personController.realRotation); 

  draw_transform(transform);
   



  vec3 prevPoint = vec3(0, personController.crouching ? Settings::hipsHeightCrouch : Settings::hipsHeightStand, 0);
  vec3 prevPointNew = prevPoint;
  
  float desiredOrientation = mod_f(personController.wantedRotation - personController.realRotation, PITWO);
  speed = glm::rotateY(speed, -personController.wantedRotation);

  auto &trajectory = animationPlayer.inputGoal.path.trajectory;
  float onPlaceError = 0;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float percentage = (i + 1.f) / AnimationTrajectory::PathLength;
    float delay = AnimationTrajectory::timeDelays[i] - (i == 0 ? 0 : AnimationTrajectory::timeDelays[i-1]);
    vec3 desiredDisplacement = speed * delay;
    vec3 trajectoryDelta = personController.desiredTrajectory[i] - prevPoint;
    prevPoint = personController.desiredTrajectory[i];
    
    vec3 adjustedTrajectoryDisplacement = lerp(trajectoryDelta, desiredDisplacement,
        1.f - exp(-MoveRate * percentage * dt));
    trajectory[i].point = prevPointNew + adjustedTrajectoryDisplacement;
    prevPointNew = trajectory[i].point;
    
    trajectory[i].rotation = lerp_angle(trajectory[i].rotation, -desiredOrientation,
                    1.f - exp(-TurnRate * percentage*dt));
    
    onPlaceError += length(adjustedTrajectoryDisplacement);
  
  }
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
    personController.desiredTrajectory[i] = trajectory[i].point;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
    trajectory[i].point = glm::rotateY(trajectory[i].point, personController.realRotation);


  animationPlayer.inputGoal.tags.clear();
  if(personController.crouching)
    animationPlayer.inputGoal.tags.insert(AnimationTag::Crouch);
  if (onPlaceError < 0.05f && + abs(desiredOrientation) * RadToDeg < 10.f)
    animationPlayer.inputGoal.tags.insert(AnimationTag::Idle);
  if (input.get_key(SDLK_SPACE) > 0)
  {

  }
}

EVENT() controller_mouse_move_handler(
  const ControllerMouseMoveEvent &e,
  PersonController &personController)
{
  float dx = e.e.dx * DegToRad * Settings::mouseSensitivity;
  personController.wantedRotation += (Settings::mouseInvertXaxis ? 1 : -1) * dx;
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