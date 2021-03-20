#include "third_person_controller.h"
#include "GameObject/game_object.h"
#include "Transform/transform.h"
#include "Animation/animation_player.h"
#include "Camera/camera.h"
#include "Time/time.h"
#include "Components/DebugTools/debug_arrow.h"
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
ThirdPersonController::ThirdPersonController(vec2 rotation, float zoom, float walk_speed, float run_speed_up):
 wantedCameraOrientation(rotation_to_orientation(rotation)), currentCameraOrientation(rotation_to_orientation(rotation)), 
 wantedCameraRotation(rotation), currentCameraRotation(rotation), 
 wantedRotation(rotation.x), currentRotation(rotation.x), 
 currentZoom(clamp_zoom(zoom)), wantedZoom(clamp_zoom(zoom)),
 crouching(false), disableEvents(false),
 walkSpeed(walk_speed), runSpeedUp(run_speed_up),
 view_offset(0)
{}

constexpr float sensitive = 0.5f;
constexpr float lerp_strength = 4.f;
constexpr float zoom_strength = 0.4f;


void ThirdPersonController::update()
{
  AnimationPlayer* player = game_object()->get_component<AnimationPlayer>();
  Transform* transform = game_object()->get_component<Transform>();
  Camera* camera = game_object()->get_component<Camera>();
  if (!player || !transform || !camera)
    return;
  currentCameraRotation = lerp(currentCameraRotation, wantedCameraRotation + vec2(PI * 0.5f * view_offset, 0), Time::delta_time() * lerp_strength);
  currentZoom = lerp(currentZoom, wantedZoom, Time::delta_time() * lerp_strength);
  currentCameraOrientation = rotation_to_orientation(currentCameraRotation);

  vec3 dir = vec3(Input::input().get_key(SDLK_d, sensitive) - Input::input().get_key(SDLK_a, sensitive), 0, Input::input().get_key(SDLK_w, sensitive) - Input::input().get_key(SDLK_s, sensitive));

  float dirAngle = (length(dir) < 0.1f)? 0 : glm::angle(vec3(0,0,1), normalize(dir));
  
  float speed = (1.f + runSpeedUp * Input::input().get_key(SDLK_LSHIFT, 0.8f)) * walkSpeed;
  const float skipSecond = (AnimationPathFeature::SkipCadres * AnimationPathFeature::PathLength) / 30.f;
  float rotationSpeed = 60 * DegToRad;
  float rotationDelta = wantedCameraRotation.x - currentRotation;
  rotationDelta += dirAngle;
  player->inputGoal.path.rotation = rotationDelta;
  if (glm::abs(rotationDelta) > skipSecond * rotationSpeed)
    rotationDelta = glm::clamp(rotationDelta, -skipSecond * rotationSpeed, skipSecond * rotationSpeed);
  
  if (glm::abs(player->rootDeltaRotation) > glm::abs(rotationDelta / skipSecond))
    currentRotation +=  rotationDelta / skipSecond * Time::delta_time();
  else
    currentRotation += player->rootDeltaRotation * Time::delta_time();

  player->inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
    player->inputGoal.tags.insert(AnimationTag::Jump);
  else
    player->inputGoal.tags.insert(crouching ? AnimationTag::Crouch : AnimationTag::Stay);
  

  for (int i = 0; i < AnimationPathFeature::PathLength; i++)
    player->inputGoal.path.path[i] = dir * speed * (i + 1.f) * (float)AnimationPathFeature::SkipCadres / 30.f;

  transform->get_position() -= 
  (player->rootDeltaTranslation.z * transform->get_forward() + 
  player->rootDeltaTranslation.y * transform->get_up()+ 
  -player->rootDeltaTranslation.x * transform->get_right()) * Time::delta_time() ;
  
  draw_transform(*transform);
  transform->set_rotation(+PI * 0.5f -currentRotation); 
  camera->get_transform().get_position() = transform->get_position() + vec3(0,1.8f,0) - currentCameraOrientation * currentZoom;
  camera->get_transform().set_rotation(PI * 0.5f - currentCameraRotation.x, -currentCameraRotation.y);
}
void ThirdPersonController::mouse_move_handler(const MouseMoveEvent &e)
{
  if (disableEvents)
    return;
  float const pixToRad = PI / 180.f * 0.2f;
  wantedCameraRotation += vec2(e.dx, -e.dy) * pixToRad;
  wantedCameraRotation.y = glm::clamp(wantedCameraRotation.y, -PI * 0.1f, PI * 0.4f);
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