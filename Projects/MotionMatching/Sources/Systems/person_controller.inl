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
  vec3 sideDir = normalize(vec3(1.f, 0.f, 0.0f)) * right;
  sideDir.z = abs(sideDir.z);
  vec3 wantedSpeed =  sideDir + vec3(0.f, 0.f, 1.f) * forward;
  float speed = length(wantedSpeed);
  if (speed > 1.f)
    wantedSpeed /= speed;
  onPlace = speed < 0.1f;
  if (!onPlace)
  {
    int moveIndex = forward > 0 ? 0 : forward < 0 ? 2 : 1;
    float runSpeeds[3] = {Settings::runForwardSpeed, Settings::runSidewaySpeed, Settings::runBackwardSpeed};
    float walkSpeeds[3] = {Settings::walkForwardSpeed, Settings::walkSidewaySpeed, Settings::walkBackwardSpeed};
    if(run > 0)
      wantedSpeed *= runSpeeds[moveIndex];
    else
      wantedSpeed *= walkSpeeds[moveIndex];
  }
  return wantedSpeed;
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
  bool onPlace;

  vec3 speed = get_wanted_speed(input, onPlace);
  if (animationTester)
  {
    //debug_log("[%f, %f, %f]", speed.x, speed.y, speed.z);input.get_key(SDLK_w)
    //debug_log("[%f]", input.get_key(SDLK_w));
  }
  float MoveRate = Settings::predictionMoveRate, TurnRate = Settings::predictionRotationRate;
  
  //float sideRot = (abs(speed.z) < 0.1f && abs(speed.x) > 0.1f) ? -speed.x * DegToRad * 5 : 0;

  float wantedRotation = personController.wantedRotation;
  float nextRootRotation = personController.realRotation - animationPlayer.rootDeltaRotation * dt;
  float nextLerpRotation = lerp_angle(personController.realRotation, wantedRotation, dt * Settings::rotationRate);
  float desiredOrientation = mod_f(wantedRotation - personController.realRotation, PITWO);
  if (rotation_abs(nextRootRotation - wantedRotation) < rotation_abs(nextLerpRotation - wantedRotation))
  {
    personController.realRotation = mod_f(nextRootRotation, PITWO); 
  }
  else
  {
    personController.realRotation = nextLerpRotation;
  }
  if (abs(desiredOrientation) * RadToDeg < 15.f)
    personController.simulatedPosition += transform.get_rotation() * glm::rotateY(speed * dt, -(wantedRotation-personController.realRotation));
  else
    personController.simulatedPosition = personController.realPosition;
  vec3 rootDelta = apply_root_motion_to_speed(speed, animationPlayer.rootDeltaTranslation);


  personController.realPosition = transform.get_position() + transform.get_rotation() * rootDelta * dt;

  vec3 positionDelta = personController.simulatedPosition - personController.realPosition;
  float errorRadius = length(positionDelta);
  if (errorRadius > Settings::maxMoveErrorRadius)
  {
    personController.realPosition += positionDelta * (errorRadius-Settings::maxMoveErrorRadius)/errorRadius;
  }
  transform.get_position() = personController.realPosition;
  transform.set_rotation(-personController.realRotation); 

  draw_transform(transform);
   
  float h = Settings::hipsHeightStand;
  if (personController.crouching)
  {
    h = animationPlayer.inputGoal.tags.find(AnimationTag::Idle) != animationPlayer.inputGoal.tags.end() ?
        Settings::hipsHeightCrouchIdle : Settings::hipsHeightCrouch;
  }

  vec3 prevPoint = vec3(0, h, 0);
  vec3 prevPointNew = prevPoint;
  
  speed = glm::rotateY(speed, -wantedRotation);

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

  if (onPlaceError < Settings::onPlaceMoveError && + abs(desiredOrientation) * RadToDeg < Settings::onPlaceRotationError)
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