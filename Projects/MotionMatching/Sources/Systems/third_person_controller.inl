#include "ecs/ecs.h"
#include "PersonController/third_person_controller.h"
#include "Transform/transform.h"
#include "Animation/animation_player.h"
#include "Time/time.h"
#include "Animation/man_property.h"
#include "PersonController/person_controller.h"


constexpr float lerp_strength = 4.f;
constexpr float zoom_strength = 0.4f;

vec3 rotation_to_orientation(vec2 rotation)
{
  float x = rotation.x;
  float y = rotation.y;
  return vec3(cos(y) * cos(x), sin(y), cos(y) * sin(x));
}
float clamp_zoom(float zoom)
{
  return glm::clamp(zoom, 0.5f, 10.f);
}

EVENT() third_controller_appear(
  const ecs::OnEntityCreated &,
  ThirdPersonController &thirdPersonController)
{
  thirdPersonController.wantedCameraOrientation = thirdPersonController.currentCameraOrientation =
      rotation_to_orientation(thirdPersonController.currentCameraRotation);

  thirdPersonController.currentZoom  = thirdPersonController.wantedZoom = clamp_zoom(thirdPersonController.currentZoom);
}
vec3 get_boost_dt(vec3 speed, float dt)
{
  float right = Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a);
  float forward = Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s);
  //forward *= 1.3f;
  bool onPlace = float_equal(right, 0) && float_equal(forward, 0);
  float run = Input::input().get_key(SDLK_LSHIFT);
  vec3 wantedSpeed(right, 0, forward);
  if (!onPlace)
  {
    wantedSpeed = normalize(wantedSpeed);
    int moveIndex = forward > 0 ? 0 : forward < 0 ? 2 : 1;
    if(run > 0)
    {
      wantedSpeed *= runSpeeds[moveIndex];
    }
    else
    {
      wantedSpeed *= walkSpeeds[moveIndex];
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

SYSTEM(ecs::SystemOrder::LOGIC) third_peson_controller_update(
  AnimationPlayer &animationPlayer,
  ecs::EntityId attachedCamera,
  PersonController &personController,
  Transform &transform,
  ThirdPersonController &thirdPersonController) 
{
  
  float dt = Time::delta_time();
  animationPlayer.update(transform, dt);
  thirdPersonController.currentCameraRotation =
  lerp(thirdPersonController.currentCameraRotation,
    thirdPersonController.wantedCameraRotation + vec2(PI * 0.5f * thirdPersonController.view_offset, 0),
    dt * lerp_strength);
  thirdPersonController.currentZoom = lerp(thirdPersonController.currentZoom, thirdPersonController.wantedZoom, dt * lerp_strength);
  thirdPersonController.currentCameraOrientation = rotation_to_orientation(thirdPersonController.currentCameraRotation);

  personController.simulatedRotation = -thirdPersonController.currentCameraRotation.x;
  personController.update_from_speed(animationPlayer, transform, personController.speed + get_boost_dt(personController.speed, dt), dt);


  float rotationDelta = (thirdPersonController.wantedCameraRotation.x) + personController.realRotation;
  

  animationPlayer.inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
  {

  }
  
  float T = AnimationTrajectory::timeDelays[AnimationTrajectory::PathLength - 1];
  vec3 hipsPoint = vec3(0, thirdPersonController.crouching ? hipsHeightCrouch : hipsHeightStand, 0);
  vec3 prevPoint = hipsPoint;
  vec3 predictedSpeed = personController.speed;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float t = AnimationTrajectory::timeDelays[i];
    float dtdelay = i == 0 ? t : t - AnimationTrajectory::timeDelays[i - 1];
    float x = - rotationDelta * (t / T); 
    animationPlayer.inputGoal.path.trajectory[i].point = prevPoint + quat(vec3(0,x,0)) * (predictedSpeed * dtdelay);
    animationPlayer.inputGoal.path.trajectory[i].rotation = x;
    prevPoint = animationPlayer.inputGoal.path.trajectory[i].point;
    predictedSpeed += get_boost_dt(predictedSpeed, dtdelay);
  }

  QUERY() update_attached_camera(attachedCamera, [&](Transform &transform)
  {
    transform.get_position() = personController.realPosition + hipsPoint - thirdPersonController.currentCameraOrientation * thirdPersonController.currentZoom;
    transform.set_rotation(PI * 0.5f - thirdPersonController.currentCameraRotation.x, -thirdPersonController.currentCameraRotation.y);
  });
}

EVENT() mouse_move_handler(
  const MouseMoveEvent &e,
  ThirdPersonController &thirdPersonController)
{
  if (thirdPersonController.disableEvents)
    return;
  float const pixToRad = PI / 180.f * 0.2f;
  thirdPersonController.wantedCameraRotation += vec2(e.dx, -e.dy) * pixToRad;
  thirdPersonController.wantedCameraRotation.y = glm::clamp(thirdPersonController.wantedCameraRotation.y, -PI * 0.45f, -PI * 0.1f);
  thirdPersonController.wantedCameraOrientation = rotation_to_orientation(thirdPersonController.wantedCameraRotation);
}

EVENT() mouse_wheel_handler(
  const MouseWheelEvent &e,
  ThirdPersonController &thirdPersonController)
{
  thirdPersonController.wantedZoom = clamp_zoom(thirdPersonController.wantedZoom + -e.wheel * zoom_strength);
}

EVENT() view_offset_handler(
  const KeyboardEvent &e,
  ThirdPersonController &thirdPersonController)
{
  switch (e.keycode)
  {
    case SDLK_LEFT:thirdPersonController.view_offset = 3; break;
    case SDLK_UP:thirdPersonController.view_offset = 2; break;
    case SDLK_RIGHT:thirdPersonController.view_offset = 1; break;
    case SDLK_DOWN:thirdPersonController.view_offset = 0; break;
  
  }
}

EVENT() disable_events_handler(
  const KeyboardEvent &e,
  ThirdPersonController &thirdPersonController)
{
  if (e.keycode == SDLK_SPACE)
    thirdPersonController.disableEvents = !thirdPersonController.disableEvents;
}

EVENT() crouch_event_handler(
  const KeyboardEvent &e,
  ThirdPersonController &thirdPersonController)
{
  if (e.keycode == SDLK_z)
    thirdPersonController.crouching = !thirdPersonController.crouching;
}