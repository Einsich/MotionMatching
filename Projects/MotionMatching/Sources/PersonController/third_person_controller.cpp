#include "third_person_controller.h"
#include "GameObject/game_object.h"
#include "Transform/transform.h"
#include "Animation/animation_player.h"
#include "Camera/camera.h"
#include "Time/time.h"
#include "Components/DebugTools/debug_arrow.h"
#include "Animation/man_property.h"

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
ThirdPersonController::ThirdPersonController(vec2 rotation, float zoom):
 wantedCameraOrientation(rotation_to_orientation(rotation)), currentCameraOrientation(rotation_to_orientation(rotation)), 
 wantedCameraRotation(rotation), currentCameraRotation(rotation), 
 wantedRotation(rotation.x), currentRotation(rotation.x), 
 currentZoom(clamp_zoom(zoom)), wantedZoom(clamp_zoom(zoom)),
 crouching(false), disableEvents(false),
 view_offset(0), speed(0)
{}

constexpr float sensitive = 0.5f;
constexpr float lerp_strength = 4.f;
constexpr float zoom_strength = 0.4f;

vec3 get_boost(vec3 speed)
{
  float right = Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a);
  float forward = Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s);

  bool onPlace = float_equal(right, 0) && float_equal(forward, 0);
  float run = Input::input().get_key(SDLK_LSHIFT);
  vec3 wantedSpeed(right, 0, forward);
  float speedLength = length(speed);
  float reaction= 1.f;
  if (!onPlace)
  {
    wantedSpeed = normalize(wantedSpeed);
    if(run > 0)
    {
      int runIndex = forward > 0 ? 0 : forward < 0 ? 2 : 1;
      wantedSpeed *= runSpeeds[runIndex];
      reaction = 1.5f;
    }
    else
    {
      wantedSpeed *= walkSpeed;
    }
  }
  vec3 dv = (wantedSpeed - speed);
  float dvLength = length(dv);
  if (dvLength < 1e-1f)
  {
    return vec3(0.f);
  }
  else
  {
    return normalize(dv) * reaction;
  }
}

void ThirdPersonController::update()
{
  AnimationPlayer* player = game_object()->get_component<AnimationPlayer>();
  Transform* transform = game_object()->get_component<Transform>();
  Camera* camera = game_object()->get_component<Camera>();
  if (!player || !transform || !camera)
    return;
  float dt = Time::delta_time();
  currentCameraRotation = lerp(currentCameraRotation, wantedCameraRotation + vec2(PI * 0.5f * view_offset, 0), dt * lerp_strength);
  currentZoom = lerp(currentZoom, wantedZoom, dt * lerp_strength);
  currentCameraOrientation = rotation_to_orientation(currentCameraRotation);

  vec3 boost = get_boost(speed);
  speed += boost * dt;
  float rotationSpeed = 60 * DegToRad;
  float rotationDelta = (wantedCameraRotation.x) - currentRotation;
  
  //if (glm::abs(rotationDelta) > DegToRad * 5)
  //  currentRotation += sign(rotationDelta) * rotationSpeed * Time::delta_time();
  currentRotation -= player->rootDeltaRotation;
  player->inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
  {

  }
  
  float T = AnimationTrajectory::timeDelays[AnimationTrajectory::PathLength - 1];
  vec3 hipsPoint = vec3(0,crouching ? hipsHeightCrouch : hipsHeightStand, 0);
  vec3 prevPoint = hipsPoint;
  vec3 predictedSpeed = speed;
  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    float t = AnimationTrajectory::timeDelays[i];
    float dt = i == 0 ? t : t - AnimationTrajectory::timeDelays[i - 1];
    float x = - rotationDelta * (t / T); 
    player->inputGoal.path.trajectory[i].point = prevPoint + quat(vec3(0,x,0)) * (predictedSpeed * dt);
    player->inputGoal.path.trajectory[i].rotation = x;
    prevPoint = player->inputGoal.path.trajectory[i].point;
    predictedSpeed += boost * dt;
  }
  transform->get_position() -= 
  (player->rootDeltaTranslation.z * transform->get_forward() + 
  player->rootDeltaTranslation.y * transform->get_up()+ 
  -player->rootDeltaTranslation.x * transform->get_right()) * dt *0.5f;
  
  draw_transform(*transform);
  transform->set_rotation(+PI * 0.5f -currentRotation); 
  camera->get_transform().get_position() = transform->get_position() + hipsPoint - currentCameraOrientation * currentZoom;
  camera->get_transform().set_rotation(PI * 0.5f - currentCameraRotation.x, -currentCameraRotation.y);
}
void ThirdPersonController::mouse_move_handler(const MouseMoveEvent &e)
{
  if (disableEvents)
    return;
  float const pixToRad = PI / 180.f * 0.2f;
  wantedCameraRotation += vec2(e.dx, -e.dy) * pixToRad;
  wantedCameraRotation.y = glm::clamp(wantedCameraRotation.y, -PI * 0.45f, -PI * 0.1f);
  wantedCameraOrientation = rotation_to_orientation(wantedCameraRotation);
}
void ThirdPersonController::mouse_wheel_handler(const MouseWheelEvent &e)
{
  wantedZoom = clamp_zoom(wantedZoom + -e.wheel * zoom_strength);
}

void ThirdPersonController::view_offset_handler(const KeyboardEvent &e)
{
  switch (e.keycode)
  {
    case SDLK_LEFT:view_offset = 3; break;
    case SDLK_UP:view_offset = 2; break;
    case SDLK_RIGHT:view_offset = 1; break;
    case SDLK_DOWN:view_offset = 0; break;
  
  }
}
void ThirdPersonController::disable_events_handler(const KeyboardEvent &)
{
  disableEvents = !disableEvents;
}
void ThirdPersonController::crouch_event_handler(const KeyboardEvent &)
{
  crouching = !crouching;
}