#include "third_person_controller.h"
#include "CommonCode/GameObject/game_object.h"
#include "../Transform/transform.h"
#include "CommonCode/Animation/animation_player.h"
#include "CommonCode/Camera/camera.h"
#include "../Time/time.h"
#include "CommonCode/Components/DebugTools/debug_arrow.h"
vec3 rotation_to_orientation(vec2 rotation)
{
  float x = rotation.x;
  float y = rotation.y;
  return vec3(cos(y) * cos(x), sin(y), cos(y) * sin(x));
}

ThirdPersonController::ThirdPersonController(vec2 rotation, float walk_speed, float run_speed_up):
 wantedOrientation(rotation_to_orientation(rotation)), currentOrientation(rotation_to_orientation(rotation)), 
 wantedRotation(rotation), currentRotation(rotation), 
 walkSpeed(walk_speed), runSpeedUp(run_speed_up),
 crouching(false)
{}

void ThirdPersonController::update()
{
  AnimationPlayer* player = game_object()->get_component<AnimationPlayer>();
  Transform* transform = game_object()->get_component<Transform>();
  Camera* camera = game_object()->get_component<Camera>();
  if (!player || !transform || !camera)
    return;
  constexpr float sensitive = 0.5f;
  vec3 dir = vec3(Input::input().get_key(SDLK_d, sensitive) - Input::input().get_key(SDLK_a, sensitive), 0, Input::input().get_key(SDLK_w, sensitive) - Input::input().get_key(SDLK_s, sensitive));
  float speed = (1.f + runSpeedUp * Input::input().get_key(SDLK_LSHIFT, 0.8f)) * walkSpeed;
  const float skipSecond = (AnimationPathFeature::SkipCadres * AnimationPathFeature::PathLength) / 30.f;
  float rotationSpeed = 60 * DegToRad;
  float rotationDelta = (wantedRotation - currentRotation).x;
  
  if (glm::abs(rotationDelta) > skipSecond * rotationSpeed)
    rotationDelta = glm::clamp(rotationDelta, -skipSecond * rotationSpeed, skipSecond * rotationSpeed);
  
  if (glm::abs(player->rootDeltaRotation) > glm::abs(rotationDelta / skipSecond))
    currentRotation.x +=  rotationDelta / skipSecond * Time::delta_time();
  else
    currentRotation.x += player->rootDeltaRotation * Time::delta_time();
 currentOrientation = rotation_to_orientation(currentRotation);

  player->inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
    player->inputGoal.tags.push_back(AnimationTag::Jump);
  else
    player->inputGoal.tags.push_back(crouching ? AnimationTag::Crouch : AnimationTag::Stay);
  
  player->inputGoal.path.rotation = rotationDelta;

  for (int i = 0; i < AnimationPathFeature::PathLength; i++)
    player->inputGoal.path.path[i] = dir * speed * (i + 1.f) * (float)AnimationPathFeature::SkipCadres / 30.f;

  transform->get_position() -= 
  (player->rootDeltaTranslation.z * transform->get_forward() + 
  player->rootDeltaTranslation.y * transform->get_up()+ 
  -player->rootDeltaTranslation.x * transform->get_right()) * Time::delta_time() ;
  vec3 p = transform->get_position();
  draw_arrow(p, p + transform->get_forward()*2.f, vec3(0,0,1), 0.1f);
  draw_arrow(p, p + transform->get_up()*2.f, vec3(0,1,0), 0.1f);
  draw_arrow(p, p + transform->get_right()*2.f, vec3(1,0,0), 0.1f);
  transform->set_rotation(+PI * 0.5f -currentRotation.x); 
  camera->get_transform().get_position() = transform->get_position() + vec3(0,1.8f,0) - currentOrientation*4.f;
  camera->get_transform().set_rotation(-PI * 0.5f - currentRotation.x, currentRotation.y);
}
void ThirdPersonController::mouse_move_handler(const MouseMoveEvent &e)
{
  float const pixToRad = PI / 180.f * 0.2f;
  wantedRotation += vec2(e.dx, e.dy) * pixToRad;
  wantedOrientation = rotation_to_orientation(wantedRotation);
}