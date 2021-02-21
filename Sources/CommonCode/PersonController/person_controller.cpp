#include "person_controller.h"
#include "CommonCode/GameObject/game_object.h"
#include "../Transform/transform.h"
#include "CommonCode/Animation/animation_player.h"
#include "../Time/time.h"


void TestPersonController::update()
{
  AnimationPlayer* player = gameObject->get_component<AnimationPlayer>();
  Transform* transform = gameObject->get_component<Transform>();
  if (!player || !transform)
    return;
  float speed = Input::input().get_key(SDLK_w, 0.5f) * (1.f + 2*Input::input().get_key(SDLK_LSHIFT, 0.8f)) * 1.2f;
  const float skipSecond = (AnimationPathFeature::SkipCadres * AnimationPathFeature::PathLength) / 30.f;
  float rotationSpeed = 60 * DegToRad;
  float rotationDelta = wantedRotation - rotation;
  
  if (glm::abs(rotationDelta) > skipSecond * rotationSpeed)
    rotationDelta = glm::clamp(rotationDelta, -skipSecond * rotationSpeed, skipSecond * rotationSpeed);
  
  if (glm::abs(player->rootDeltaRotation) > glm::abs(rotationDelta / skipSecond))
    rotation +=  rotationDelta / skipSecond * Time::delta_time();
  else
    rotation += player->rootDeltaRotation * Time::delta_time();
  player->inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
    player->inputGoal.tags.push_back(AnimationTag::Jump);
  else
    player->inputGoal.tags.push_back(crouching ? AnimationTag::Crouch : AnimationTag::Stay);
  
  player->inputGoal.path.rotation = rotationDelta;

  for (int i = 0; i < AnimationPathFeature::PathLength; i++)
    player->inputGoal.path.path[i] = vec3(0, 0, 1) * speed * (i + 1.f) * (float)AnimationPathFeature::SkipCadres / 30.f;

  transform->get_position() -= (player->rootDeltaTranslation.z * transform->get_forward() + player->rootDeltaTranslation.x * transform->get_right()) * Time::delta_time() ;
  transform->set_rotation(rotation); 
  
  //player->get_state_machine().set_rotation(r);
  //player->get_state_machine().set_speed(speed);
}

void TestPersonController::rotate(const KeyboardEvent &event)
{
  float r = DegToRad * 90.f;
  if (event.keycode == SDLK_a)
    wantedRotation += r;
  if (event.keycode == SDLK_d)
    wantedRotation -= r;
}
void TestPersonController::crouch(const KeyboardEvent &event)
{
  crouching = !crouching;
  //player->get_state_machine().crouch(crouching);
}
void TestPersonController::jump(const KeyboardEvent &event)
{
  //player->get_state_machine().jump();

}